#ifndef COMBUSTER
#define COMBUSTER
#include<cmath>
#include<vector>
#include"jiading.h"
#include"Gassolve.h"
double miu_i(std::vector<std::vector<double>>HS_a,int i, double T, double p,int L)
{
    double Ro=8.3144;
    if (i < L)
    {
        double Ho_iT = Ro * (HS_a[i][0] * T + HS_a[i][1] * pow(T, 2) / 2 + HS_a[i][2] * pow(T, 3) / 3 + HS_a[i][3] * pow(T, 4) / 4 + HS_a[i][4] * pow(T, 5) / 5 + HS_a[i][5]); // 焓
        double So_iT = Ro * (HS_a[i][0] * log(T) + HS_a[i][1] * T + HS_a[i][2] * pow(T, 2) / 2 + HS_a[i][3] * pow(T, 3) / 3 + HS_a[i][4] * pow(T, 4) / 4 + HS_a[i][6]);        // 熵
        return Ho_iT - T * So_iT;
    }
    else
    {
        double Ho_iT = Ro * (HS_a[i][0] * T + HS_a[i][1] * pow(T, 2) / 2 + HS_a[i][2] * pow(T, 3) / 3 + HS_a[i][3] * pow(T, 4) / 4 + HS_a[i][4] * pow(T, 5) / 5 + HS_a[i][5]); // 焓
        double So_iT = Ro * (HS_a[i][0] * log(T) + HS_a[i][1] * T + HS_a[i][2] * pow(T, 2) / 2 + HS_a[i][3] * pow(T, 3) / 3 + HS_a[i][4] * pow(T, 4) / 4 + HS_a[i][6]);        // 熵
        return Ho_iT - T * So_iT + Ro * T * log(p);
    }
}
double cal_yi(std::vector<std::vector<double>>HS_a,int i, double T, double p,int L)
{
    double Ro=8.3144;
    double miu = miu_i(HS_a,i, T, p,L);
    return -miu / (Ro * T);
}
std::vector<double> Combuster_cal(int P,int N,int M,int L,std::vector<std::vector<double>>HS_a,std::vector<std::vector<double>>St_aij,std::vector<double>gi,std::vector<double>M_k,std::vector<std::vector<double>>Aij,std::vector<double>cc,double p,double T_min,double T_max,double delta_T,double pre_H_total)
{
    std::vector<double> jiading_ans = jiading(P,M,St_aij,gi,M_k);
    struct H_t
    {
        double H_all,Temp;
        std::vector<double>end_mol;
    }H_temp[200];
    int top=0;
    double Ro = 8.3144;
    for (double T = T_min; T <= T_max; T += delta_T)
    {
        std::vector<double>c(N);
        c = cc;
        for (int times = 1; times <= 100; times++)
        {
            double cg = 0;
            for (int i = L; i < N; i++)
                cg += c[i];
            std::vector<double> y(N); // N=8
            for (int i = 0; i < N; i++)
                y[i] = cal_yi(HS_a,i, T,p,L);
            std::vector<double>Phi_c(N); // N=8
            for (int i = 0; i < N; i++)
                Phi_c[i] = (-y[i] + log(c[i]) + log(p) - log(cg)) * c[i];
            std::vector<double> AtimesPhi(M); // M=3
            for (int k = 0; k < M; k++)
            {
                AtimesPhi[k] = 0;
                for (int j = L; j < N; j++)
                {
                    AtimesPhi[k] += Aij[k][j] * Phi_c[j];
                }
            }
            std::vector<std::vector<double>> R(M,std::vector<double>(M)); // M=3
            for (int k = 0; k < M; k++)
            {
                for (int i = 0; i < M; i++)
                {
                    R[k][i] = 0;
                    for (int j = L; j < N; j++)
                    {
                        R[k][i] += Aij[k][j] * Aij[i][j] * c[j];
                    }
                }
            }
            std::vector<double> alpha(3); // M=3
            for (int k = 0; k < M; k++)
            {
                alpha[k] = 0;
                for (int j = L; j < N; j++)
                    alpha[k] += Aij[k][j] * c[j];
            }
            std::vector<std::vector<double>> Aleft(M+1+L,std::vector<double>(M+1+L));
            std::vector<double> Bright(M+L+1);
            for (int i = 0; i < M + 1 + L; i++)
            {
                for (int j = 0; j < M + 1 + L; j++)
                {
                    if (i < M && j < M)
                        Aleft[i][j] = R[i][j];
                    else if (i < M && j == M)
                        Aleft[i][j] = alpha[i];
                    else if (i < M && j > M)
                        Aleft[i][j] = Aij[i][j - (M + 1)];
                    else if (i == M && j < M)
                        Aleft[i][j] = alpha[j];
                    else if (i > M && j < M)
                        Aleft[i][j] = Aij[j][i - (M + 1)];
                    else
                        Aleft[i][j] = 0;
                }
            }
            for (int i = 0; i < M + 1 + L; i++)
            {
                if (i < M)
                {
                    Bright[i] = jiading_ans[i];
                    for (int j = L; j < N; j++)
                        Bright[i] += Aij[i][j] * Phi_c[j];
                }
                else if (i == M)
                {
                    Bright[i] = 0;
                    for (int j = L; j < N; j++)
                        Bright[i] += Phi_c[j];
                }
                else
                    Bright[i] = -y[i - M - 1];
            }
            
            std::vector<double> ans_X = solveGaussian(Aleft, Bright);

            double Xg = cg * ans_X[M];
            std::vector<double> X_new(N);//N=8
            for (int i = 0; i < N; i++)
            {
                if (i < L)
                    X_new[i] = ans_X[i + M + 1];
                else
                {
                    X_new[i] = -Phi_c[i] + c[i] * ans_X[M];
                    double cnt = 0;
                    for (int k = 0; k < M; k++)
                    {
                        cnt += Aij[k][i] * ans_X[k];
                    }
                    X_new[i] += cnt * c[i];
                }
            }
            for (int i = 0; i < N; i++)
            {
                if (X_new[i] > 0)
                    c[i] = X_new[i];
                else
                    c[i] = 1e-7;
            }
        }
        double H_total = 0;
        for (int i = 0; i < N; i++)
        {
            H_total += c[i] * Ro * (HS_a[i][0] * T + HS_a[i][1] * pow(T, 2) / 2 + HS_a[i][2] * pow(T, 3) / 3 + HS_a[i][3] * pow(T, 4) / 4 + HS_a[i][4] * pow(T, 5) / 5 + HS_a[i][5]);
        }
        H_temp[top].H_all = H_total;
        H_temp[top].Temp=T;
        H_temp[top].end_mol = c;
        top++;
    }
    bool flag=false;
    std::vector<double> ans_T_mol(N+1);
    for(int i=1;i<top;i++)
    {
        if(pre_H_total>=H_temp[i-1].H_all&&pre_H_total<=H_temp[i].H_all)
        {
            double ans_temp,ratio;
            ratio=(pre_H_total-H_temp[i-1].H_all)/(H_temp[i].H_all-H_temp[i-1].H_all);
            ans_temp = H_temp[i-1].Temp+(H_temp[i].Temp-H_temp[i-1].Temp)*ratio;
            ans_T_mol[0]=ans_temp;
            for(int k=0;k<N;k++)
            {
                ans_T_mol[k+1]=H_temp[i-1].end_mol[k]+(H_temp[i].end_mol[k]-H_temp[i-1].end_mol[k])*ratio;
            }
            flag=true;
        }
    }
    if(!flag)
    {
        ans_T_mol[0]=-1;
    }
    return ans_T_mol;
}
std::vector<double> Min_Gibbis_cal(int P,int N,int M,int L,std::vector<std::vector<double>>HS_a,std::vector<std::vector<double>>St_aij,std::vector<double>gi,std::vector<double>M_k,std::vector<std::vector<double>>Aij,std::vector<double>cc,double p,double T)
{
    std::vector<double> jiading_ans = jiading(P,M,St_aij,gi,M_k);
    struct H_t
    {
        double H_all,Temp;
        std::vector<double>end_mol;
    }H_temp[200];
    int top=0;
    double Ro = 8.3144;
    std::vector<double>c(N);
    c = cc;
    for (int times = 1; times <= 100; times++)
    {
        double cg = 0;
        for (int i = L; i < N; i++)
            cg += c[i];
        std::vector<double> y(N); // N=8
        for (int i = 0; i < N; i++)
            y[i] = cal_yi(HS_a,i, T,p,L);
        std::vector<double>Phi_c(N); // N=8
        for (int i = 0; i < N; i++)
            Phi_c[i] = (-y[i] + log(c[i]) + log(p) - log(cg)) * c[i];
        std::vector<double> AtimesPhi(M); // M=3
        for (int k = 0; k < M; k++)
        {
            AtimesPhi[k] = 0;
            for (int j = L; j < N; j++)
            {
                AtimesPhi[k] += Aij[k][j] * Phi_c[j];
            }
        }
        std::vector<std::vector<double>> R(M,std::vector<double>(M)); // M=3
        for (int k = 0; k < M; k++)
        {
            for (int i = 0; i < M; i++)
            {
                R[k][i] = 0;
                for (int j = L; j < N; j++)
                {
                    R[k][i] += Aij[k][j] * Aij[i][j] * c[j];
                }
            }
        }
        std::vector<double> alpha(3); // M=3
        for (int k = 0; k < M; k++)
        {
            alpha[k] = 0;
            for (int j = L; j < N; j++)
                alpha[k] += Aij[k][j] * c[j];
        }
        std::vector<std::vector<double>> Aleft(M+1+L,std::vector<double>(M+1+L));
        std::vector<double> Bright(M+L+1);
        for (int i = 0; i < M + 1 + L; i++)
        {
            for (int j = 0; j < M + 1 + L; j++)
            {
                if (i < M && j < M)
                    Aleft[i][j] = R[i][j];
                else if (i < M && j == M)
                    Aleft[i][j] = alpha[i];
                else if (i < M && j > M)
                    Aleft[i][j] = Aij[i][j - (M + 1)];
                else if (i == M && j < M)
                    Aleft[i][j] = alpha[j];
                else if (i > M && j < M)
                    Aleft[i][j] = Aij[j][i - (M + 1)];
                else
                    Aleft[i][j] = 0;
            }
        }
        for (int i = 0; i < M + 1 + L; i++)
        {
            if (i < M)
            {
                Bright[i] = jiading_ans[i];
                for (int j = L; j < N; j++)
                    Bright[i] += Aij[i][j] * Phi_c[j];
            }
            else if (i == M)
            {
                Bright[i] = 0;
                for (int j = L; j < N; j++)
                    Bright[i] += Phi_c[j];
            }
            else
                Bright[i] = -y[i - M - 1];
        }
        
        std::vector<double> ans_X = solveGaussian(Aleft, Bright);

        double Xg = cg * ans_X[M];
        std::vector<double> X_new(N);//N=8
        for (int i = 0; i < N; i++)
        {
            if (i < L)
                X_new[i] = ans_X[i + M + 1];
            else
            {
                X_new[i] = -Phi_c[i] + c[i] * ans_X[M];
                double cnt = 0;
                for (int k = 0; k < M; k++)
                {
                    cnt += Aij[k][i] * ans_X[k];
                }
                X_new[i] += cnt * c[i];
            }
        }
        for (int i = 0; i < N; i++)
        {
            if (X_new[i] > 0)
                c[i] = X_new[i];
            else
                c[i] = 1e-7;
        }
    }
    return c;
}
#endif