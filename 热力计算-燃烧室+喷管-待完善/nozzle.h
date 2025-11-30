#ifndef NOZZLE
#define NOZZLE

#include"combuster.h"
#include"relichanshu.h"
#include<cmath>

std::vector<double> From_pe(int P,int N,int M,int L,std::vector<std::vector<double>>HS_a,std::vector<std::vector<double>>St_aij,std::vector<double>gi,std::vector<double>M_k,std::vector<std::vector<double>>Aij,std::vector<double>cc,double pc,double Tc,double gamma_s,double S_pre,double pe)
{
    double Te_guess = Tc*pow((pe/pc),(gamma_s-1)/gamma_s);
    double T_min = Te_guess-500;
    double T_max = Te_guess+500;
    double delta_T = 50;
    std::vector<double>end_mol;
    struct S_i
    {
        double S_total;
        double Te;
        std::vector<double>mol_out;
    }S_temp[100];
    int top=0;
    for(int Te=T_min;Te<=T_max;Te+=delta_T)
    {
        end_mol=Min_Gibbis_cal(P,N,M,L,HS_a,St_aij,gi,M_k,Aij,cc,pe,Te);
        double ng=0;
        for(int i=L;i<N;i++)ng+=end_mol[i];
        double S_try = S_end_solve(N,L,Te,pe,ng,end_mol,HS_a);
        S_temp[top].S_total = S_try;
        S_temp[top].Te = Te;
        S_temp[top].mol_out = end_mol;
        top++;
    }
    bool flag=false;
    std::vector<double>ans(N+1);
    for(int k=1;k<top;k++)
    {
        if((S_pre>=S_temp[k-1].S_total&&S_pre<=S_temp[k].S_total)||(S_pre<=S_temp[k-1].S_total&&S_pre>=S_temp[k].S_total))
        {
            double S1 = S_temp[k-1].S_total;
            double S2 = S_temp[k].S_total;
            double T1 = S_temp[k-1].Te;
            double T2 = S_temp[k].Te;
            flag=true;
            ans[N]=T1+((S_pre-S1)/(S2-S1))*(T2-T1);
            for(int i=0;i<N;i++)ans[i] = S_temp[k-1].mol_out[i]+((S_pre-S1)/(S2-S1))*(S_temp[k].mol_out[i]-S_temp[k-1].mol_out[i]);
            return ans;
        }
    }
    ans[N] = -1;
    return ans;  
}
std::vector<double> From_Te(int P,int N,int M,int L,std::vector<std::vector<double>>HS_a,std::vector<std::vector<double>>St_aij,std::vector<double>gi,std::vector<double>M_k,std::vector<std::vector<double>>Aij,std::vector<double>cc,double pc,double Tc,double gamma_s,double Te)
{
    double pe = pc * pow(Te/Tc,gamma_s/(gamma_s-1));
    std::vector<double>end_mol = Min_Gibbis_cal(P,N,M,L,HS_a,St_aij,gi,M_k,Aij,cc,pe,Te);
    std::vector<double>ans(N+1);
    for(int i=0;i<N;i++)ans[i] = end_mol[i];
    ans[N] = pe;
    return ans;
}
#endif