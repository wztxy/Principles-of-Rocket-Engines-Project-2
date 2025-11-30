#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <iomanip>
using namespace std;
// H2O H2 OH H CO2 CO O2 O
// H C O
double HS_a[8][7] = {{4.19864056E+00, -2.03643410E-03, 6.52040211E-06, -5.48797062E-09, 1.77197817E-12, -3.02937267E+04, -8.49032208E-01}, // H2O
                     {2.34433112E+00, 7.98052075E-03, -1.94781510E-05, 2.01572094E-08, -7.37611761E-12, -9.17935173E+02, 6.83010238E-01},  // H2
                     {3.99201543E+00, -2.40131752E-03, 4.61793841E-06, -3.88113333E-09, 1.36411470E-12, 3.61508056E+03, -1.03925458E-01},  // OH
                     {2.50000000E+00, 7.05332819E-13, -1.99591964E-15, 2.30081632E-18, -9.27732332E-22, 2.54736599E+04, -4.46682853E-01},  // H
                     {2.35677352E+00, 8.98459677E-03, -7.12356269E-06, 2.45919022E-09, -1.43699548E-13, -4.83719697E+04, 9.90105222E+00},  // CO2
                     {3.57953347E+00, -6.10353680E-04, 1.01681433E-06, 9.07005884E-10, -9.04424499E-13, -1.43440860E+04, 3.50840928E+00},  // CO
                     {3.78245636E+00, -2.99673416E-03, 9.84730201E-06, -9.68129509E-09, 3.24372837E-12, -1.06394356E+03, 3.65767573E+00},  // O2
                     {3.16826710E+00, -3.27931884E-03, 6.64306396E-06, -6.12806624E-09, 2.11265971E-12, 2.91222592E+04, 2.05193346E+00}};  // O
double jiading_ans[3] = {50, 12.5, 50};
double M_k[3] = {1.0, 12.0, 16.0};
double Aij[3][8] = {{2, 2, 1, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 1, 0, 0}, {1, 0, 1, 0, 2, 1, 2, 1}};
int L = 0, N = 8, M = 3;
double Ro = 8.3144;
double Aleft[30][30], Bright[30];
double miu_i(int i, double T, double p)
{
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
double cal_yi(int i, double T, double p)
{
    double miu = miu_i(i, T, p);
    return -miu / (Ro * T);
}
vector<double> solveGaussian(vector<vector<double>> A, vector<double> B)
{
    int n = A.size();

    // 构造增广矩阵 [A|B]
    vector<vector<double>> aug(n, vector<double>(n + 1));
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
            aug[i][j] = A[i][j];
        aug[i][n] = B[i];
    }

    // 高斯消元
    for (int i = 0; i < n; ++i)
    {
        // 选主元（部分主元选择）
        int maxRow = i;
        for (int k = i + 1; k < n; ++k)
            if (abs(aug[k][i]) > abs(aug[maxRow][i]))
                maxRow = k;
        swap(aug[i], aug[maxRow]);

        // 如果主元为0，矩阵奇异
        if (abs(aug[i][i]) < 1e-10)
        {
            throw runtime_error("矩阵奇异，无解或有无穷多解。");
        }

        // 消元
        for (int k = i + 1; k < n; ++k)
        {
            double factor = aug[k][i] / aug[i][i];
            for (int j = i; j <= n; ++j)
                aug[k][j] -= factor * aug[i][j];
        }
    }

    // 回代求解
    vector<double> x(n);
    for (int i = n - 1; i >= 0; --i)
    {
        x[i] = aug[i][n];
        for (int j = i + 1; j < n; ++j)
            x[i] -= aug[i][j] * x[j];
        x[i] /= aug[i][i];
    }

    return x;
}

int main()
{
    freopen("outdata.csv", "w", stdout);
    double p = 18.0;
    for (double T = 2300; T <= 3500; T += 50)
    {
        double c[8] = {22, 1e-7, 1, 1e-7, 11, 1, 1e-7, 1e-7}; // 初始值,N=8
        for (int times = 1; times <= 100; times++)
        {
            double cg = 0;
            for (int i = L; i < N; i++)
                cg += c[i];
            double y[8]; // N=8
            for (int i = 0; i < N; i++)
                y[i] = cal_yi(i, T, p);
            double Phi_c[8]; // N=8
            for (int i = 0; i < N; i++)
                Phi_c[i] = (-y[i] + log(c[i]) + log(p) - log(cg)) * c[i];
            double AtimesPhi[3]; // M=3
            for (int k = 0; k < M; k++)
            {
                AtimesPhi[k] = 0;
                for (int j = L; j < N; j++)
                {
                    AtimesPhi[k] += Aij[k][j] * Phi_c[j];
                }
            }
            double R[3][3]; // M=3
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
            // cout<<"R:"<<endl;
            /*for(int k=0;k<M;k++)
            {
                for(int i=0;i<M;i++)
                    cout<<R[k][i]<<' ';
                cout<<endl;
            }*/
            double alpha[3]; // M=3
            for (int k = 0; k < M; k++)
            {
                alpha[k] = 0;
                for (int j = L; j < N; j++)
                    alpha[k] += Aij[k][j] * c[j];
            }
            // cout<<"Aleft:"<<endl;
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
                    // cout<<Aleft[i][j]<<' ';
                }
                // cout<<endl;
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
            vector<vector<double>> Aleft_vec(M + 1 + L, vector<double>(M + 1 + L));
            for (int i = 0; i < M + 1 + L; ++i)
                for (int j = 0; j < M + 1 + L; ++j)
                    Aleft_vec[i][j] = Aleft[i][j];

            vector<double> Bright_vec(M + 1 + L);
            for (int i = 0; i < M + 1 + L; i++)
                Bright_vec[i] = Bright[i];
            // cout<<"Bright_vec"<<endl;
            // for(int i=0;i<M+1+L;i++)cout<<Bright_vec[i]<<endl;
            vector<double> ans_X = solveGaussian(Aleft_vec, Bright_vec);
            // cout<<"ans_X:"<<endl;
            // for(int i=0;i<M+1+L;i++)
            //    cout<<ans_X[i]<<' ';

            double Xg = cg * ans_X[M];
            double X_new[8];//N=8
            // cout<<"--------times: "<<times<<"-------"<<endl;
            // cout<<"X_new:"<<endl;
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
                // cout<<X_new[i]<<',';
            }
            // cout<<endl;
            for (int i = 0; i < N; i++)
            {
                if (X_new[i] > 0)
                    c[i] = X_new[i];
                else
                    c[i] = 1e-7;
            }
        }
        cout << T << ',';
        for (int i = 0; i < N; i++)
            cout << c[i] << ',';
        double H_total = 0;
        for (int i = 0; i < N; i++)
        {
            H_total += c[i] * Ro * (HS_a[i][0] * T + HS_a[i][1] * pow(T, 2) / 2 + HS_a[i][2] * pow(T, 3) / 3 + HS_a[i][3] * pow(T, 4) / 4 + HS_a[i][4] * pow(T, 5) / 5 + HS_a[i][5]);
        }
        cout << H_total << endl;
    }
}