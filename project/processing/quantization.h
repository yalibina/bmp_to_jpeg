#pragma once

#include "matrix.h"
#include <algorithm>

class Quant {
public:
    static Matrix<double> applyQuantization(const Matrix<double> &matrix, size_t size_of_block, double q);

private:
    static Matrix<double> getQMatrix(size_t N, double q);

};


