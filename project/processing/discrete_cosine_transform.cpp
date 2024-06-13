#include "discrete_cosine_transform.h"

# define PI 3.14159265358979323846

Matrix DCT::getDctMatrix(size_t N) {
    Matrix result(N, N, 0);

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            if (i == 0) {
                result(i, j) = 1 / sqrt((double) N);
                continue;
            }
            result(i, j) =
                    sqrt(2.0 / (double) N) * cos((2.0 * (double) j + 1.0) * (double) i * PI / (2.0 * (double) N));
        }
    }
    return result;
}

Matrix DCT::applyDiscreteCosineTransform(const Matrix &matrix, size_t size_of_block) {
    Matrix result(matrix.getRowSize(), matrix.getColumnSize());
    Matrix DCT_matrix = getDctMatrix(size_of_block);
    Matrix DCT_matrix_transposed = DCT_matrix.transposed();
    size_t blocks_in_column = matrix.getColumnSize() / size_of_block + (matrix.getColumnSize() % size_of_block != 0);
    size_t blocks_in_row = matrix.getRowSize() / size_of_block + (matrix.getRowSize() % size_of_block != 0);
    for (size_t i = 0; i < blocks_in_column; ++i) {
        for (size_t j = 0; j < blocks_in_row; ++j) {
            Matrix block = matrix.getSlice({i * size_of_block, j * size_of_block},
                                           {(i + 1) * size_of_block, (j + 1) * size_of_block});
            block.resize(size_of_block, size_of_block, 0);

            Matrix res_block = DCT_matrix * block * DCT_matrix_transposed;
            result.insert({i * size_of_block, j * size_of_block}, res_block);
        }
    }
    return result;
}

