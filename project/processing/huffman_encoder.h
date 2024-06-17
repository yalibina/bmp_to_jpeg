#pragma once

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
    byte codelengths[17] = {0};
    byte symbols[176] = {0};
};

// Node structure for Huffman Tree
struct Node {
    std::pair<uint8_t, uint8_t> data;
    int freq;
    Node *left, *right;

    Node(std::pair<uint8_t, uint8_t> data, int freq);
};

// Comparator for priority queue
struct compare {
    bool operator()(Node *l, Node *r);
};


class Huffman {
public:
    static HuffmanTable huffman_encoder(std::vector<current> &sequence);

private:
    static Node *buildHuffmanTree(std::map<std::pair<uint8_t, uint8_t>, int> &frequency);

    static void count_codelengths(Node *v, byte (&codelengths)[17], byte depth = -1, bool onlyones = true);

    static uint to_byte(std::pair<uint8_t, uint8_t> p);

    static std::map<std::pair<uint8_t, uint8_t>, byte>
    fill_htable(HuffmanTable &htable, std::vector<std::pair<int, std::pair<uint8_t, uint8_t>>> &sorted_symbols);
};

