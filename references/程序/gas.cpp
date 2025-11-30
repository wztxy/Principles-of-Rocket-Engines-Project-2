#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// 高斯消元法求解 Ax = B
vector<double> solveGaussian(vector<vector<double>> A, vector<double> B) {
    int n = A.size();

    // 构造增广矩阵 [A|B]
    vector<vector<double>> aug(n, vector<double>(n + 1));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            aug[i][j] = A[i][j];
        aug[i][n] = B[i];
    }

    // 高斯消元
    for (int i = 0; i < n; ++i) {
        // 选主元（部分主元选择）
        int maxRow = i;
        for (int k = i + 1; k < n; ++k)
            if (abs(aug[k][i]) > abs(aug[maxRow][i]))
                maxRow = k;
        swap(aug[i], aug[maxRow]);

        // 如果主元为0，矩阵奇异
        if (abs(aug[i][i]) < 1e-10) {
            throw runtime_error("矩阵奇异，无解或有无穷多解。");
        }

        // 消元
        for (int k = i + 1; k < n; ++k) {
            double factor = aug[k][i] / aug[i][i];
            for (int j = i; j <= n; ++j)
                aug[k][j] -= factor * aug[i][j];
        }
    }

    // 回代求解
    vector<double> x(n);
    for (int i = n - 1; i >= 0; --i) {
        x[i] = aug[i][n];
        for (int j = i + 1; j < n; ++j)
            x[i] -= aug[i][j] * x[j];
        x[i] /= aug[i][i];
    }

    return x;
}

int main() {
    int n;
    cout << "请输入矩阵维度 n: ";
    cin >> n;

    vector<vector<double>> A(n, vector<double>(n));
    vector<double> B(n);

    cout << "请输入矩阵 A（" << n << "x" << n << "）：\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> A[i][j];

    cout << "请输入向量 B（" << n << "维）：\n";
    for (int i = 0; i < n; ++i)
        cin >> B[i];

    try {
        vector<double> x = solveGaussian(A, B);
        cout << "解向量 x 为：\n";
        for (int i = 0; i < n; ++i)
            cout << "x[" << i << "] = " << fixed << setprecision(6) << x[i] << '\n';
    } catch (const exception& e) {
        cerr << "错误: " << e.what() << '\n';
    }

    return 0;
}
