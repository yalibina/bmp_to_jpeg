#pragma once

#include "matrix.h"
#include "huffman_encoder.h"

#include <cmath>
#include <map>

typedef struct {
    std::vector<current> DC_sequence;
    std::vector<std::vector<current>> AC_sequence;
    HuffmanTable DC_table;
    HuffmanTable AC_table;
} EncodedMatrix;

class Entropy {
public:
    static EncodedMatrix encode_matrix(Matrix<int> &M, size_t size_of_block);
    static void entropy_encoder(Matrix<int> &Y, Matrix<int> &Cb, Matrix<int> &Cr);

private:
    static std::pair<int, std::vector<int>> zig_zag(Matrix<int> &block, size_t size_of_block);
    static uint8_t bitsize(int n);
    static std::vector<current> RLE(std::vector<int> &sequence);

};

