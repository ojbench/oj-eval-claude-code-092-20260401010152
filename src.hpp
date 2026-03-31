#ifndef SRC_HPP
#define SRC_HPP

#include "fraction.hpp"

// 如果你不需要使用 matrix 类，请将 IGNORE_MATRIX 改为 0
#define IGNORE_MATRIX 1

#if IGNORE_MATRIX

class matrix {
private:

    // m行n列的矩阵，用动态二维数组存储，每个元素是分数类实例
    int m, n;
    fraction **data;

    //****************************
    // TODO: 你可以在此添加任何需要的类成员和函数。
    //       你可以任意修改 matrix 类框架的任何类成员和函数。
    //****************************

public:

    //****************************
    // TODO: 你可以在此添加任何需要的类成员和函数。
    //       你可以任意修改 matrix 类框架的任何类成员和函数。
    //****************************

    // 默认构造函数
    matrix() {
        m = n = 0;
        data = nullptr;
    }

    // 构造函数，构建 m_*n_ 的矩阵，矩阵元素设为0。
    matrix(int m_, int n_) {
        m = m_;
        n = n_;
        if (m <= 0 || n <= 0) {
            data = nullptr;
            return;
        }
        data = new fraction*[m];
        for (int i = 0; i < m; i++) {
            data[i] = new fraction[n];
            for (int j = 0; j < n; j++) {
                data[i][j] = fraction(0);
            }
        }
    }

    // 拷贝构造函数，构建与 obj 完全相同的矩阵。
    matrix(const matrix &obj) {
        m = obj.m;
        n = obj.n;
        if (m <= 0 || n <= 0 || obj.data == nullptr) {
            data = nullptr;
            return;
        }
        data = new fraction*[m];
        for (int i = 0; i < m; i++) {
            data[i] = new fraction[n];
            for (int j = 0; j < n; j++) {
                data[i][j] = obj.data[i][j];
            }
        }
    }

    // 移动拷贝构造函数。
    matrix(matrix &&obj) noexcept {
        m = obj.m;
        n = obj.n;
        data = obj.data;
        obj.m = 0;
        obj.n = 0;
        obj.data = nullptr;
    }

    // 析构函数。
    ~matrix() {
        if (data != nullptr) {
            for (int i = 0; i < m; i++) {
                delete[] data[i];
            }
            delete[] data;
        }
    }

    // 重载赋值号。
    matrix &operator=(const matrix &obj) {
        if (this == &obj) {
            return *this;
        }
        // 释放旧数据
        if (data != nullptr) {
            for (int i = 0; i < m; i++) {
                delete[] data[i];
            }
            delete[] data;
        }
        // 拷贝新数据
        m = obj.m;
        n = obj.n;
        if (m <= 0 || n <= 0 || obj.data == nullptr) {
            data = nullptr;
            return *this;
        }
        data = new fraction*[m];
        for (int i = 0; i < m; i++) {
            data[i] = new fraction[n];
            for (int j = 0; j < n; j++) {
                data[i][j] = obj.data[i][j];
            }
        }
        return *this;
    }

    // 重载括号，返回矩阵的第i行(1-based)、第j列(1-based)的元素的引用。如果 i、j 不合法，抛出 matrix_error 错误。
    fraction &operator()(int i, int j) {
        if (i < 1 || i > m || j < 1 || j > n) {
            throw matrix_error();
        }
        return data[i-1][j-1];
    }

    // 重载乘号，返回矩阵乘法 lhs * rhs 的结果。如果 lhs 的列数与 rhs 的行数不相等，抛出 matrix_error 错误。
    friend matrix operator*(const matrix &lhs, const matrix &rhs) {
        if (lhs.n != rhs.m) {
            throw matrix_error();
        }
        matrix result(lhs.m, rhs.n);
        for (int i = 0; i < lhs.m; i++) {
            for (int j = 0; j < rhs.n; j++) {
                fraction sum(0);
                for (int k = 0; k < lhs.n; k++) {
                    sum = sum + lhs.data[i][k] * rhs.data[k][j];
                }
                result.data[i][j] = sum;
            }
        }
        return result;
    }

    // 返回矩阵的转置。若矩阵为空，抛出 matrix_error 错误。
    matrix transposition() {
        if (m <= 0 || n <= 0 || data == nullptr) {
            throw matrix_error();
        }
        matrix result(n, m);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                result.data[j][i] = data[i][j];
            }
        }
        return result;
    }

    // 返回矩阵的行列式。建议用高斯消元实现。若矩阵不是方阵或为空，抛出 matrix_error 错误。
    fraction determination() {
        if (m != n || m <= 0 || data == nullptr) {
            throw matrix_error();
        }

        // 创建临时矩阵用于高斯消元
        matrix temp(*this);
        fraction det(1);

        // 高斯消元
        for (int i = 0; i < n; i++) {
            // 找到第i列的主元
            int pivot = i;
            for (int j = i + 1; j < n; j++) {
                // 简单比较：找到绝对值最大的
                if (temp.data[j][i].operator==(fraction(0)) == false) {
                    if (temp.data[pivot][i].operator==(fraction(0))) {
                        pivot = j;
                    }
                }
            }

            // 如果主元为0，行列式为0
            if (temp.data[pivot][i].operator==(fraction(0))) {
                return fraction(0);
            }

            // 交换行
            if (pivot != i) {
                for (int j = 0; j < n; j++) {
                    fraction tmp = temp.data[i][j];
                    temp.data[i][j] = temp.data[pivot][j];
                    temp.data[pivot][j] = tmp;
                }
                det = det * fraction(-1);
            }

            // 消元
            for (int j = i + 1; j < n; j++) {
                if (temp.data[j][i].operator==(fraction(0)) == false) {
                    fraction factor = temp.data[j][i] / temp.data[i][i];
                    for (int k = i; k < n; k++) {
                        temp.data[j][k] = temp.data[j][k] - factor * temp.data[i][k];
                    }
                }
            }
        }

        // 计算对角线元素的乘积
        for (int i = 0; i < n; i++) {
            det = det * temp.data[i][i];
        }

        return det;
    }

    // 辅助函数：删除指定行和列
    matrix removeRowCol(int row, int col) const {
        if (row < 1 || row > m || col < 1 || col > n) {
            throw matrix_error();
        }
        matrix result(m - 1, n - 1);
        int ri = 0;
        for (int i = 0; i < m; i++) {
            if (i == row - 1) continue;
            int rj = 0;
            for (int j = 0; j < n; j++) {
                if (j == col - 1) continue;
                result.data[ri][rj] = data[i][j];
                rj++;
            }
            ri++;
        }
        return result;
    }
};

#endif

class resistive_network {
private:

    // 节点数量 和 接线数量
    int interface_size, connection_size;

    // 邻接矩阵A，电导矩阵C，Laplace矩阵(A^tCA) (具体定义见 reading_materials.pdf)
    matrix adjacency, conduction, laplace;

public:

    //****************************
	// 注意，请勿私自修改以下函数接口的声明！
    //****************************

    // 设置电阻网络。节点数量为interface_size_，接线数量为connection_size_。
    //       对于 1<=i<=connection_size_，从节点from[i-1]到节点to[i-1]有接线，对应电阻为resistance[i-1]。
    //       保证接线使得电阻网络联通，from[i-1] < to[i-1]，resitance[i-1] > 0，均合法。
    resistive_network(int interface_size_, int connection_size_, int from[], int to[], fraction resistance[]) {
        interface_size = interface_size_;
        connection_size = connection_size_;

        // 构建邻接矩阵 A (m x n)
        adjacency = matrix(connection_size, interface_size);
        for (int i = 1; i <= connection_size; i++) {
            int from_node = from[i-1];
            int to_node = to[i-1];
            adjacency(i, from_node) = fraction(1);
            adjacency(i, to_node) = fraction(-1);
        }

        // 构建电导矩阵 C (m x m) - 对角矩阵
        conduction = matrix(connection_size, connection_size);
        for (int i = 1; i <= connection_size; i++) {
            conduction(i, i) = fraction(1) / resistance[i-1];
        }

        // 构建 Laplace 矩阵 L = A^T * C * A
        matrix A_T = adjacency.transposition();
        matrix temp = A_T * conduction;
        laplace = temp * adjacency;
    }

    ~resistive_network() = default;

    // 返回节点 interface_id1 和 interface_id2 (1-based)之间的等效电阻。
    //       保证 interface_id1 <= interface_id2 均合法。
    fraction get_equivalent_resistance(int interface_id1, int interface_id2) {
        // R_ij = det(M_ij) / det(M_i)
        // M_i: 去掉第i行和第i列
        // M_ij: 去掉第i行、第i列、第j行、第j列

        int i = interface_id1;
        int j = interface_id2;

        if (i == j) {
            return fraction(0);
        }

        // 计算 det(M_i)
        matrix M_i = laplace.removeRowCol(i, i);
        fraction det_Mi = M_i.determination();

        // 计算 det(M_ij)
        // 先去掉第i行和第i列，然后去掉第j行和第j列
        // 注意：去掉第i行后，如果j > i，索引需要调整
        matrix temp = laplace.removeRowCol(i, i);
        int j_adjusted = (j > i) ? (j - 1) : j;
        matrix M_ij = temp.removeRowCol(j_adjusted, j_adjusted);
        fraction det_Mij = M_ij.determination();

        return det_Mij / det_Mi;
    }

    // 在给定节点电流I的前提下，返回节点id(1-based)的电压。认为节点interface_size(1-based)的电压为0。
    //       对于 1<=i<=interface_size，节点i(1-based)对应电流为 current[i-1]。
    //       保证 current 使得电阻网络有解，id < interface_size 合法。
    fraction get_voltage(int id, fraction current[]) {
        // u_i = det(M_i^n) / det(M_n)
        // M_n: 去掉第n行和第n列
        // M_i^n: 将A^TCA的第i列替换成I，然后去掉第n行和第n列

        int n = interface_size;
        int i = id;

        // 计算 det(M_n)
        matrix M_n = laplace.removeRowCol(n, n);
        fraction det_Mn = M_n.determination();

        // 构建 M_i^n: 将 laplace 的第i列替换成 current
        matrix L_modified = laplace;
        for (int row = 1; row <= n; row++) {
            L_modified(row, i) = current[row - 1];
        }

        // 去掉第n行和第n列
        matrix M_i_n = L_modified.removeRowCol(n, n);
        fraction det_Mi_n = M_i_n.determination();

        return det_Mi_n / det_Mn;
    }


    // 在给定节点电压U的前提下，返回电阻网络的功率。
    //       对于 1<=i<=interface_size，节点i (1-based) 对应电压为 voltage[i-1]。
    //       保证 voltage 合法。
    fraction get_power(fraction voltage[]) {
        // P = Σ(u_wi^2 / r_i) for i=1 to m
        // u_w = A * U^T

        // 构建电压向量矩阵 U (n x 1)
        matrix U(interface_size, 1);
        for (int i = 1; i <= interface_size; i++) {
            U(i, 1) = voltage[i - 1];
        }

        // 计算 u_w = A * U
        matrix u_w = adjacency * U;

        // 计算功率 P = Σ(u_wi^2 / r_i)
        fraction power(0);
        for (int i = 1; i <= connection_size; i++) {
            fraction u_wi = u_w(i, 1);
            fraction r_i = fraction(1) / conduction(i, i);
            power = power + (u_wi * u_wi) / r_i;
        }

        return power;
    }
};


#endif //SRC_HPP