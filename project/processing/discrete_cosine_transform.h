#pragma once

#include "matrix.h"

#include <cmath>

class DCT {
public:
    static Matrix getDctMatrix(size_t N);

    static Matrix applyDiscreteCosineTransform(const Matrix &matrix, size_t size_of_block);
};
