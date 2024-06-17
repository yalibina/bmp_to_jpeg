#include "quantization.h"

Matrix<int> Quant::getQMatrix(size_t N, int q) {
    Matrix<int> Q(N, N, 0);
    Matrix<int> Q_base({{16, 11, 10, 16, 24, 40, 51, 61},
                           {12, 12, 14, 19, 26, 58, 60, 55},
                           {14, 13, 16, 24, 40, 57, 69, 56},
                           {14, 17, 22, 29, 51, 87, 80, 62},
                           {18, 22, 37, 56, 68, 109, 103, 77},
                           {24, 35, 55, 64, 81, 104, 113, 92},
                           {49, 64, 78, 87, 103, 121, 120, 101},
                           {72, 92, 95, 98, 112, 100, 103, 99}});
    if (q == 50) {
        return Q_base;
    }
    int s;
    if (q < 50) {
        s = 5000 / q;
    } else {
        s = 200 - 2 * q;
    }

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            Q(i, j) = std::max(1, static_cast<int>(std::round((s * Q_base(i, j) + 50) / 100)));
        }
    }
    return Q;
}

Matrix<int> Quant::applyQuantization(const Matrix<double> &matrix, size_t size_of_block, int q) {
    Matrix<int> result(matrix.getRowSize(), matrix.getColumnSize());
    Matrix<int> Q = getQMatrix(size_of_block, q);

    size_t blocks_in_column = matrix.getColumnSize() / size_of_block + (matrix.getColumnSize() % size_of_block != 0);
    size_t blocks_in_row = matrix.getRowSize() / size_of_block + (matrix.getRowSize() % size_of_block != 0);
    for (size_t i = 0; i < blocks_in_column; ++i) {
        for (size_t j = 0; j < blocks_in_row; ++j) {
            Matrix block = matrix.getSlice({i * size_of_block, j * size_of_block},
                                           {(i + 1) * size_of_block, (j + 1) * size_of_block});
            block.resize(size_of_block, size_of_block, 0);

            Matrix<int> res_block(Q.getRowSize(), Q.getColumnSize());
            for (size_t n = 0; n < Q.getColumnSize(); ++n) {
                for (size_t m = 0; m < Q.getRowSize(); ++m) {
                    res_block(n, m) = static_cast<int>(std::round(block(n, m) / Q(n, m)));
                }
            }
            result.insert({i * size_of_block, j * size_of_block}, res_block);
        }
    }

    return result;
}
