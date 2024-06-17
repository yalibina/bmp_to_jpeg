#include "huffman_encoder.h"

// Node structure for Huffman Tree
struct Node {
    std::pair<uint8_t, uint8_t> data;
    int freq;
    Node *left, *right;

    Node(std::pair<uint8_t, uint8_t> data, int freq) {
        left = right = nullptr;
        this->data = data;
        this->freq = freq;
    }
};

// Comparator for priority queue
struct compare {
    bool operator()(Node *l, Node *r) {
        return (l->freq > r->freq);
    }
};

// Function to build Huffman Tree and generate Huffman codes
Node* buildHuffmanTree(std::map<std::pair<uint8_t, uint8_t>, int> &frequency) {
    Node *left, *right, *top;
    std::priority_queue<Node *, std::vector<Node *>, compare> minHeap;

    for (auto v: frequency) {
        minHeap.push(new Node(v.first, v.second));
    }

    while (minHeap.size() != 1) {
        left = minHeap.top();
        minHeap.pop();
        right = minHeap.top();
        minHeap.pop();

        top = new Node({0, 0}, left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }

    auto root = minHeap.top();
    return root;
}

void count_codelengths(Node* v, byte (&codelengths)[17], byte depth = 1, bool onlyones = true) {
    if (!v->left && !v->right) {
        ++codelengths[depth + onlyones];
    } else {
        count_codelengths(v->left, codelengths, depth + 1, false);
        count_codelengths(v->right, codelengths, depth + 1, onlyones);
    }
}

uint to_byte(std::pair<uint8_t, uint8_t> p) {
    uint res = (p.first << 4) + p.second;
    return res;
}

std::map<std::pair<uint8_t, uint8_t>, byte> fill_htable(HuffmanTable &htable, std::vector<std::pair<int, std::pair<uint8_t, uint8_t>>> &sorted_symbols) {
    uint code = 0;
    uint offset = 0;
    std::map<std::pair<uint8_t, uint8_t>, byte> symbol_to_code;
    for (uint i = 0; i < 16; ++i) {
        for (uint j = 0; j < htable.codelengths[i]; ++j) { // if offsets, rewrite
            symbol_to_code[sorted_symbols[offset].second] = code;
            htable.symbols[offset] = to_byte(sorted_symbols[offset].second);
            ++offset;
            ++code;
        }
        code <<= 1;
    }
    return symbol_to_code;
}

// Input: sequence of currents; output: huffman table; function updates enc_rls in currents;
HuffmanTable huffman_encoder(std::vector<current> &sequence) {
    std::map<std::pair<uint8_t, uint8_t>, int> frequency;

    HuffmanTable htable; // the answer huf table is here

    for (auto c: sequence) {
        ++frequency[{c.RUNLENGTH, c.SIZE}];
    }

    // std::map<std::pair<uint8_t, uint8_t>, std::string> HuffmanTable;
    auto root = buildHuffmanTree(frequency);
    count_codelengths(root, htable.codelengths);

    std::vector<std::pair<int, std::pair<uint8_t, uint8_t>>> sorted_symbols(frequency.size());
    for (auto current : frequency) {
        sorted_symbols.emplace_back(current.second, current.first);
    }
    std::sort(sorted_symbols.begin(), sorted_symbols.end(),
              [](const std::pair<int, std::pair<int8_t, int8_t>>& a,
                      const std::pair<int, std::pair<int8_t, int8_t>>& b) {
        return a.first > b.first;
    });

    auto symbol_to_code = fill_htable(htable, sorted_symbols);

    std::vector<byte> encoded_sequence(sequence.size());
    for (auto c : sequence) {
        c.ENC_RLS = symbol_to_code[{c.RUNLENGTH, c.SIZE}];
    }
    return htable;
}

