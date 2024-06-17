#pragma once

#include "matrix.h"
#include <algorithm>

class Quant {
public:
    static Matrix<int> applyQuantization(const Matrix<double> &matrix, size_t size_of_block, double q);

private:
    static Matrix<int> getQMatrix(size_t N, double q);

};


