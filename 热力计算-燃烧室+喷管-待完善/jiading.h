// ...existing code...
#ifndef JIADING_H
#define JIADING_H

#include <vector>
#include <cmath>

// header-only 实现：inline 避免多重定义
inline std::vector<double> jiading(int P, int M,
    const std::vector<std::vector<double>>& St_aij,
    const std::vector<double>& gi,
    const std::vector<double>& Atom_j)
{
    std::vector<double> M_i(P, 0.0);
    for (int i = 0; i < P; ++i)
    {
        for (int j = 0; j < M; ++j)
            M_i[i] += Atom_j[j] * St_aij[i][j];
        if (std::fabs(M_i[i]) < 1e-12) M_i[i] = 1e-12;
    }

    std::vector<std::vector<double>> Bij(P, std::vector<double>(M, 0.0));
    for (int i = 0; i < P; ++i)
        for (int j = 0; j < M; ++j)
            Bij[i][j] = St_aij[i][j] * 1000.0 / M_i[i];

    std::vector<double> ans(M, 0.0);
    for (int j = 0; j < M; ++j)
        for (int i = 0; i < P; ++i)
            ans[j] += gi[i] * Bij[i][j];

    return ans;
}

#endif // JIADING_H
// ...existing code...