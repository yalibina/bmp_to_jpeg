#pragma once

#include "matrix.h"

#include <cmath>

class DCT {
public:
    static Matrix<double> applyDiscreteCosineTransform(const Matrix<double> &matrix, size_t size_of_block);

private:
    static Matrix<double> getDctMatrix(size_t N);
};
