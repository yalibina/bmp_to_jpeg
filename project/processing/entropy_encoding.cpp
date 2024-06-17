#include "entropy_encoding.h"

// Block 8x8 -> {AC, DC} from zigzag
std::pair<int, std::vector<int>> Entropy::zig_zag(Matrix<int> &block) {
    std::vector<int> sequence;
    bool direction_up = true;
    for (int diag = 1; diag < 15; ++diag) {
        for (int i = 0; i <= diag; ++i) {
            int j = diag - i;
            if (i >= 0 && i < 8 && j >= 0 && j < 8) {
                if (direction_up) {
                    sequence.push_back(block(i, j));

                } else {
                    sequence.push_back(block(j, i));
                }
            }
        }
        direction_up = !direction_up;
    }
    return std::make_pair(block(0, 0), sequence);
}

uint8_t Entropy::bitsize(int n) {
    uint8_t size = 0;
    n = abs(n);
    while (n) {
        ++size;
        n >>= 1;
    }
    return size;
}

// current sequence of one block -> RLE encoded with (0, 0) at the end. Don't forget that 15 nulls is maximum.
std::vector<current> Entropy::RLE(std::vector<int> &sequence) {
    std::vector <current> encoded_sequence;
    uint8_t null_count = 0;
    for (auto k: sequence) {
        if (k == 0) {
            ++null_count;
        } else {
            while (null_count >= 15) {
                encoded_sequence.push_back({15, 0, 0, 0});
                null_count -= 15;
            }
            encoded_sequence.push_back({null_count, bitsize(k), 0, k});
            null_count = 0;
        }
    }
    encoded_sequence.push_back({0, 0, 0, 0}); // Wiki says that here should be (0, 0)
    return encoded_sequence;
}

// Full matrix (Y/Cr/Cb) -> vector<int> DiffDC and vector<int> RLE(current) for each block
EncodedMatrix Entropy::encode_matrix(Matrix<int> &M) {
    EncodedMatrix encodedMatrix;
    int prevDC = 0;
    size_t n = M.getColumnSize();
    size_t m = M.getRowSize();
    for (int i = 0; i < n / 8; ++i) {
        for (int j = 0; j < m / 8; ++j) {
            auto block = M.getSlice({i * 8, j * 8},
                                    {i * 8 + 7, j * 8 + 7});
            auto blockACDC = zig_zag(block);
            encodedMatrix.DC_sequence.push_back({0, bitsize(blockACDC.first - prevDC),
                                        0, blockACDC.first - prevDC}); // Add diffDC
            prevDC = blockACDC.first;
            encodedMatrix.AC_sequence.emplace_back(RLE(blockACDC.second)); // Add RLE(vector<current>)
        }
    }

    std::vector<current> flattened_AC; // хахах сори за такие приколы
    for (auto &ac_block : encodedMatrix.AC_sequence) {
        for (auto AC : ac_block) {
            flattened_AC.push_back(AC);
        }
    }
    encodedMatrix.AC_table = Huffman::huffman_encoder(flattened_AC);
    int i = 0;
    for (auto &ac_block : encodedMatrix.AC_sequence) {
        for (auto &AC : ac_block) {
            AC.ENC_RLS = flattened_AC[i].ENC_RLS;
            ++i;
        }
    }
    encodedMatrix.DC_table = Huffman::huffman_encoder(encodedMatrix.DC_sequence);
    return encodedMatrix;
}

void Entropy::entropy_encoder(Matrix<int> &Y, Matrix<int> &Cb, Matrix<int> &Cr) {
    auto EncY = encode_matrix(Y);
    auto EncCb = encode_matrix(Cb);
    auto EncCr = encode_matrix(Cr);
}
