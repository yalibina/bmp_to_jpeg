#include <vector>
#include <map>
#include <queue>
#include <iostream>
#include <cstdint>
#include <bitset>
#include <sstream>

typedef struct {
    uint8_t RUNLENGTH; // Number of nulls before non-null current value
    uint8_t SIZE; // Size (in bits) of non-null current value
    int AMPLITUDE; // Quantized current-value itself
} current; // current is kept here too (for universal usage in Huffman). In that case RL = 0;

typedef struct {
    std::map<std::pair<uint8_t, uint8_t>, std::string> HuffmanTable;
} huffman_table;

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

// Utility function to store Huffman codes in a map
void storeCodes(Node *root, std::string str, std::map<std::pair<uint8_t, uint8_t>, std::string> &HuffmanTable) {
    if (root == nullptr)
        return;
    if (!root->left && !root->right)
        HuffmanTable[root->data] = str;
    storeCodes(root->left, str + "0", HuffmanTable);
    storeCodes(root->right, str + "1", HuffmanTable);
}

// Function to build Huffman Tree and generate Huffman codes
void buildHuffmanTree(std::map<std::pair<uint8_t, uint8_t>, int> &frequency,
                      std::map<std::pair<uint8_t, uint8_t>, std::string> &HuffmanTable) {
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

    storeCodes(minHeap.top(), "", HuffmanTable);
}

huffman_table huffman_encoder(std::vector<current> sequence) {
    std::map<std::pair<uint8_t, uint8_t>, int> frequency;

    for (auto c: sequence) {
        ++frequency[{c.RUNLENGTH, c.SIZE}];
    }

    std::map<std::pair<uint8_t, uint8_t>, std::string> HuffmanTable;
    buildHuffmanTree(frequency, HuffmanTable);

    huffman_table result;
    result.HuffmanTable = HuffmanTable;

    return result;
}

//int main() {
//    std::vector<current> sequence = {
//            {0, 2, 3},
//            {1, 3, 5},
//            {0, 2, -1},
//            {2, 4, 7},
//            {0, 2, 3},
//            {0, 2, 3},
//            {0, 2, 3},
//            {1, 3, 5},
//            {1, 3, 5},
//            {10, 2, 2}
//    };
//
//    huffman_table encoded_sequence = huffman_encoder(sequence);
//
//    std::cout << "Huffman Table:" << std::endl;
//    for (const auto &kv: encoded_sequence.HuffmanTable) {
//        std::cout << "(" << static_cast<int>(kv.first.first) << ", " << static_cast<int>(kv.first.second) << "): "
//                  << kv.second << std::endl;
//    }
//
//    return 0;
//}
