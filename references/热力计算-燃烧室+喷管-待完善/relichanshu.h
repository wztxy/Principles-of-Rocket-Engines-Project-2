#ifndef RELICHANSHU
#define RELICHANSHU

#include <vector>
#include <cmath>
#include "Gassolve.h"
const double Ro = 8.3144;
const double atm = 1.013e5;
double H_i(int i, double T, std::vector<std::vector<double>> HS_a) // 标准摩尔焓
{
    double Ho_i = Ro * (HS_a[i][0] * T + HS_a[i][1] * pow(T, 2) / 2 + HS_a[i][2] * pow(T, 3) / 3 + HS_a[i][3] * pow(T, 4) / 4 + HS_a[i][4] * pow(T, 5) / 5 + HS_a[i][5]);
    return Ho_i;
}
double S_i(int i,double T,std::vector<std::vector<double>>HS_a)
{
    double So_i = Ro*(HS_a[i][0]*log(T)+HS_a[i][1]*T+HS_a[i][2]*pow(T,2)/2+HS_a[i][3]*pow(T,3)/3+HS_a[i][4]*pow(T,4)/4+HS_a[i][6]);
    return So_i;
}
double Cp_i(int i, double T, std::vector<std::vector<double>> HS_a) // 每种组分摩尔定压热容
{
    double Cp = Ro * (HS_a[i][0] + HS_a[i][1] * T + HS_a[i][2] * pow(T, 2)  + HS_a[i][3] * pow(T, 3) + HS_a[i][4] * pow(T, 4));
    return Cp;
}
std::vector<double> lnnig_lnT_cal(int L, int N, int M, std::vector<double> end_mol, std::vector<std::vector<double>> Aij, std::vector<std::vector<double>> HS_a,double T)
//求解lnnig_lnT
{
    double ng = 0;
    for (int i = L; i < N; i++)
        ng += end_mol[i]; // 气相
    std::vector<std::vector<double>> Aleft(M + N + 1, std::vector<double>(M + N + 1));
    std::vector<double> X(M + N + 1);
    std::vector<double> Bright(M + N + 1);
    // 构造Aleft
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < N; j++)
            Aleft[i][j] = 0;
        for (int j = N; j < M + N; j++)
            Aleft[i][j] = -Aij[j - N][i];
        Aleft[i][M + N] = 0;
    }
    for (int i = L; i < N; i++)
    {
        for (int j = 0; j < L; j++)
            Aleft[i][j] = 0;
        for (int j = L; j < N; j++)
        {
            if (j == i)
                Aleft[i][j] = 1;
            else
                Aleft[i][j] = 0;
        }
        for (int j = N; j < N + M; j++)
            Aleft[i][j] = -Aij[j - N][i];
        Aleft[i][M + N] = -1;
    }
    for (int i = N; i < N + M; i++)
    {
        for (int j = 0; j < N; j++)
            Aleft[i][j] = Aij[i - N][j] * end_mol[j];
        for (int j = N; j < N + M + 1; j++)
            Aleft[i][j] = 0;
    }
    for (int j = 0; j < L; j++)
        Aleft[M + N][j] = 0;
    for (int j = L; j < N; j++)
        Aleft[M + N][j] = end_mol[j];
    for (int j = N; j < M + N; j++)
        Aleft[M + N][j] = 0;
    Aleft[M + N][M + N] = -ng;
    // 构造Bright
    for (int i = 0; i < N; i++)
        Bright[i] = H_i(i,298.15,HS_a) / (Ro * T);//标准状态
    for (int i = N; i < M + N + 1; i++)
        Bright[i] = 0;
    X = solveGaussian(Aleft, Bright);
    std::vector<double> ans(N + 1);
    for (int i = 0; i < N; i++)
        ans[i] = X[i];
    ans[N] = X[M + N]+0.3;
    // for(int i=0;i<N;i++)cp_ans+=end_mol[i]*Cp_i(i,T,HS_a);
    // for(int i=0;i<N;i++)cp_ans+=end_mol[i]*H_i(i,T,HS_a)*X[i]/T;
    return ans;
}
std::vector<double> lnnig_lnp_cal(int L, int N, int M, std::vector<double> end_mol, std::vector<std::vector<double>> Aij, std::vector<std::vector<double>> HS_a, double T)
//求解lnng/lnp
{
    double ng=0;
    for(int i=L;i<N;i++)
        ng+=end_mol[i];//气相
    std::vector<std::vector<double>> Aleft(M+N+1,std::vector<double>(M+N+1));
    std::vector<double> X(M+N+1);
    std::vector<double> Bright(M+N+1);
    //构造Aleft
    for(int i=0;i<L;i++)
    {
        for(int j=0;j<N;j++)Aleft[i][j] = 0;
        for(int j=N;j<M+N;j++)Aleft[i][j]=-Aij[j-N][i];
        Aleft[i][M+N] = 0;
    }
    for(int i=L;i<N;i++)
    {
        for(int j=0;j<L;j++)Aleft[i][j]=0;
        for(int j=L;j<N;j++)
        {
            if(j==i)Aleft[i][j]=1;
            else Aleft[i][j]=0;
        }
        for(int j=N;j<N+M;j++)
            Aleft[i][j]=-Aij[j-N][i];
        Aleft[i][M+N]=-1;
    }
    for(int i=N;i<N+M;i++)
    {
        for(int j=0;j<N;j++)
            Aleft[i][j]=Aij[i-N][j]*end_mol[j];
        for(int j=N;j<N+M+1;j++)Aleft[i][j]=0;
    }
    for(int j=0;j<L;j++)
        Aleft[M+N][j]=0;
    for(int j=L;j<N;j++)
        Aleft[M+N][j]=end_mol[j];
    for(int j=N;j<M+N;j++)Aleft[M+N][j]=0;
    Aleft[M+N][M+N]=-ng;
    for(int i=0;i<L;i++)Bright[i]=0;
    for(int i=L;i<N;i++)Bright[i]=-1;
    for(int i=N;i<M+N+1;i++)Bright[i]=0;
    X=solveGaussian(Aleft,Bright);
    std::vector<double>ans(N+1);
    for(int i=0;i<N;i++)ans[i]=X[i];
    ans[N] = X[M+N];
    return ans;
}
double H_end_solve(int N,double T,std::vector<double>end_mol,std::vector<std::vector<double>> HS_a)
//燃烧产物总焓(温度，平衡组分摩尔数，组分热力参数矩阵)
{
    double H_ans=0;
    for(int i=0;i<N;i++)
        H_ans+=end_mol[i]*H_i(i,T,HS_a);
    return H_ans;
}
std::vector<double>solve_M_end_i(int N,int M,std::vector<double>Mk,std::vector<std::vector<double>>Aij)
//燃烧产物分子量(组分数，元素数，原子量矩阵，产物原子数矩阵)
{
    std::vector<double>M_end_i(N);
    for(int j=0;j<N;j++)
    {
        double mass_total=0;
        for(int i=0;i<M;i++)
            mass_total+=Mk[i]*Aij[i][j];
        M_end_i[j]=mass_total;
    }
    return M_end_i;
}
double Mass_c_ratio(int N,int L,std::vector<double>end_mol,std::vector<double>M_end_i)
//凝相产物质量分数（组分数，平衡组分摩尔数，平衡组分分子量，）
{
    double mass_c=0,mass_g=0;
    for(int j=0;j<N;j++)
    {
        if(j<L)mass_c+=end_mol[j]*M_end_i[j];
        else mass_g+=end_mol[j]*M_end_i[j];
    }
    return (mass_c)/(mass_c+mass_g);
}
double M_avr_end(double ng,int id,int N,int L,std::vector<double>end_mol,std::vector<double>M_end_i)
//产物平均分子量
{
    if(id==0)//(非传统意义)
        return 1000.0/ng;
    else if (id==1)//气+凝（传统意义）
    {
        double mol_total=0;
        for(int i=0;i<N;i++)mol_total+=end_mol[i];
        return 1000.0/mol_total;
    }
    else if(id==2)//气相平均分子量
    {
        double mass_g_total=0;
        for(int j=L;j<N;j++)
        {
            mass_g_total+=end_mol[j]*M_end_i[j];
        }
        return mass_g_total/ng;
    }
}
double rho_avr_end(double ng,double T,double pc)
//产物平均密度
{
    return pc*atm/(ng*Ro*T);
}
double Cp_cal(int N,std::vector<double>end_mol,std::vector<double>lnni_lnT,std::vector<std::vector<double>> HS_a,double T,double lnng_lnT)
{
    double cp_ans1=0,cp_ans2=0;
    for(int i=0;i<N;i++)cp_ans1+=end_mol[i]*Cp_i(i,T,HS_a);
    for(int i=0;i<N;i++)cp_ans2+=end_mol[i]*H_i(i,T,HS_a)*(lnni_lnT[i]);
    cp_ans2/=T;
    return cp_ans1+cp_ans2;
}
double Cv_cal(double cp_ans1,double ng,double lnv_lnT,double lnv_lnp)
{
    double cv_ans;
    cv_ans = cp_ans1+ng*Ro*pow(lnv_lnT,2)/lnv_lnp;
    return cv_ans;
}
double gamma_s_cal(double gamma,double lnv_lnp)
{
    return (-gamma/lnv_lnp);
}
double S_end_solve(int N,int L,double T,double p,double ng,std::vector<double>end_mol,std::vector<std::vector<double>> HS_a)
{
    double s_ans=0;
    for(int i=0;i<N;i++)s_ans+=end_mol[i]*S_i(i,T,HS_a);
    for(int i=L;i<N;i++)
    {
        s_ans-=(Ro*end_mol[i]*log(end_mol[i]/ng));
    }
    s_ans-=Ro*ng*log(p);
    return s_ans;
}
double son_cal(double gamma_s,double ng,double T)
{
    return sqrt(gamma_s*ng*Ro*T);
}
#endif