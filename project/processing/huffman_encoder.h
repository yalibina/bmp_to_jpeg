#ifndef BMP_TO_JPEG_HUFFMAN_ENCODER_H
#define BMP_TO_JPEG_HUFFMAN_ENCODER_H

#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <iostream>
#include <cstdint>
#include <bitset>
#include <sstream>

typedef unsigned char byte;
typedef unsigned int uint;

typedef struct {
    uint8_t RUNLENGTH; // Number of nulls before non-null current value
    uint8_t SIZE; // Size (in bits) of non-null current value
    byte ENC_RLS; // Encoded runlength and size
    int AMPLITUDE; // Quantized current-value itself
} current; // DC is kept here too (for universal usage in Huffman). In that case RL = 0;

struct HuffmanTable {
    byte codelengths[17] = { 0 };
    byte symbols[176] = { 0 };
};

HuffmanTable huffman_encoder(std::vector<current> &sequence);

#endif //BMP_TO_JPEG_HUFFMAN_ENCODER_H
