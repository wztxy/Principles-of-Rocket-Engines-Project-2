#include<iostream>
#include<cstdio>
#include<cstring>
#include"combuster.h"
#include"nozzle.h"
#include"relichanshu.h"
using namespace std;
int P,N,M,L;
vector<vector<double>>HS_a(100,vector<double>(10));
vector<vector<double>>St_aij(50,vector<double>(50));
vector<vector<double>>Aij(50,vector<double>(100));
vector<double>M_k(50);
vector<double>gi(50);
vector<double>cc(50);
double p,T_min,T_max,delta_T,pe,H_pre,Te;

int main()
{
    /*---------------读入部分--------------------*/
    freopen("RS-25in.in","r",stdin);
    cin>>P>>M>>N>>L;//读入含义见（读入说明.md）
    for(int i=0;i<P;i++)
        for(int j=0;j<M;j++)
            cin>>St_aij[i][j];
    for(int i=0;i<P;i++)cin>>gi[i];
    for(int i=0;i<M;i++)cin>>M_k[i];
    for(int i=0;i<N;i++)
        for(int j=0;j<7;j++)
            cin>>HS_a[i][j];
    for(int i=0;i<M;i++)
        for(int j=0;j<N;j++)cin>>Aij[i][j];
    cin>>p;
    cin>>T_min>>T_max>>delta_T;
    for(int i=0;i<N;i++)cin>>cc[i];
    cin>>H_pre;
    /*---------------燃烧室求解部分--------------------*/
    vector<double> ans_cb (N+1);
    double T;
    ans_cb = Combuster_cal(P,N,M,L,HS_a,St_aij,gi,M_k,Aij,cc,p,T_min,T_max,delta_T,H_pre);
    printf("燃烧温度：%.3lf\n",ans_cb[0]);
    for(int i=1;i<N+1;i++)printf("平衡组分%d: %.6lf\n",i,ans_cb[i]);
    printf("\n");
    vector<double>end_mol(N);
    for(int i=1;i<N+1;i++)end_mol[i-1]=ans_cb[i];
    T = ans_cb[0];
    double ng=0;
    for(int i=L;i<N;i++)ng+=end_mol[i];
    /*---------------产物参数计算部分--------------------*/
    double H_end = H_end_solve(N,T,end_mol,HS_a);
    vector<double>M_end_i(N);
    M_end_i = solve_M_end_i(N,M,M_k,Aij);
    double mass_c_ratio = Mass_c_ratio(N,L,end_mol,M_end_i);
    double mass_g_ratio = 1-mass_c_ratio;
    double M_avr_0 = M_avr_end(ng,0,N,L,end_mol,M_end_i);
    double M_avr_1 = M_avr_end(ng,1,N,L,end_mol,M_end_i);
    double M_avr_2 = M_avr_end(ng,2,N,L,end_mol,M_end_i);
    double rho_avr = rho_avr_end(ng,T,p);
    double lnng_lnT,lnng_lnp,lnv_lnT,lnv_lnp;
    vector<double>lnnig_lnT(N+1),lnnig_lnp(N+1);
    lnnig_lnT = lnnig_lnT_cal(L,N,M,end_mol,Aij,HS_a,T);
    lnng_lnT = lnnig_lnT[N];
    lnnig_lnp = lnnig_lnp_cal(L,N,M,end_mol,Aij,HS_a,T);
    lnng_lnp = lnnig_lnp[N];
    lnv_lnT = 1+lnng_lnT;
    lnv_lnp = -1 + lnng_lnp;
    double cp,cv;
    cp = Cp_cal(N,end_mol,lnnig_lnT,HS_a,T,lnng_lnT);
    cv = Cv_cal(cp,ng,lnv_lnT,lnv_lnp);
    double gamma = cp/cv;
    double gamma_s = gamma_s_cal(gamma,lnv_lnp);
    double s_c = S_end_solve(N,L,T,p,ng,end_mol,HS_a);
    double son = son_cal(gamma_s,ng,T);
    printf("燃烧产物焓(kJ/kg): %.6lf\n",H_end/1000);
    printf("燃烧室温度(K): %.6lf\n",T);
    printf("凝相产物质量分数: %.3lf\n",mass_c_ratio);
    printf("气相产物质量分数: %.3lf\n",mass_g_ratio);
    printf("燃烧产物平均摩尔质量(g/mol): %.3lf\n",M_avr_0);
    printf("燃烧产物平均密度(kg/m^3):%.3lf\n",rho_avr);
    printf("传统意义燃烧产物平均摩尔质量(g/mol): %.3lf\n",M_avr_1);
    printf("气相平均摩尔质量(g/mol):%.3lf\n",M_avr_2);
    printf("dLnv/dLnp: %.6lf\n",lnv_lnp);
    printf("dLnv/dLnT: %.6lf\n",lnv_lnT);
    printf("定压比热(kJ/(kg·K)): %.6lf\n",cp/1000);
    printf("定容比热(kJ/kg·K): %.6lf\n",cv/1000);
    printf("比热比: %.6lf\n",gamma);
    printf("等熵指数:%.6lf\n",gamma_s);
    printf("燃烧产物熵(kJ/(kgK)): %.6lf\n",s_c/1000);
    printf("声速(m/s): %.6lf\n",son);
    /*---------------喷管求解部分--------------------*/
    int id;
    cin>>id;
    if(id == 1)
    {
        cin>>pe;
        vector<double>out_ans;
        out_ans = From_pe(P,N,M,L,HS_a,St_aij,gi,M_k,Aij,cc,p,T,gamma_s,s_c,pe);
        double Te = out_ans[N];
        printf("由出口压强计算\n");
        printf("截面压强为: %.3lf\n",pe);
        printf("截面温度为: %.3lf\n",Te);
        printf("截面组分: \n");
        for(int i=0;i<N;i++)printf("组分%d: %.6lf\n",i+1,out_ans[i]);
    }
    if(id == 2)
    {
        cin>>Te;
        vector<double>out_ans;
        out_ans = From_Te(P,N,M,L,HS_a,St_aij,gi,M_k,Aij,cc,p,T,gamma_s,Te);
        printf("截面压强为: %.6lf",out_ans[N]);
        printf("截面组分: \n");
        for(int i=0;i<N;i++)printf("组分%d: %.6lf\n",i+1,out_ans[i]);
    }
    return 0;
}