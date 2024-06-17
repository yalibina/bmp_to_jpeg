#include "entropy_encoding.h"

// Block 8x8 -> {AC, DC} from zigzag
std::pair<int, std::vector<int>> Entropy::zig_zag(Matrix<int> &block, size_t size_of_block) {
    std::vector<int> sequence;
    bool direction_up = true;
    for (int diag = 1; diag < (2 * size_of_block - 1); ++diag) {
        for (int i = 0; i <= diag; ++i) {
            int j = diag - i;
            if (i >= 0 && i < size_of_block && j >= 0 && j < size_of_block) {
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
EncodedMatrix Entropy::encode_matrix(Matrix<int> &M, size_t size_of_block) {
    EncodedMatrix encodedMatrix;
    int prevDC = 0;
    size_t blocks_in_column = M.getColumnSize() / size_of_block + (M.getColumnSize() % size_of_block != 0);
    size_t blocks_in_row = M.getRowSize() / size_of_block + (M.getRowSize() % size_of_block != 0);
    for (int i = 0; i < blocks_in_column; ++i) {
        for (int j = 0; j < blocks_in_row; ++j) {
            auto block = M.getSlice({i * size_of_block, j * size_of_block},
                                         {(i + 1) * size_of_block, (j + 1) * size_of_block});
            block.resize(size_of_block, size_of_block, 0);
            auto blockACDC = zig_zag(block, size_of_block);
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
    auto EncY = encode_matrix(Y, 8);
    auto EncCb = encode_matrix(Cb, 8);
    auto EncCr = encode_matrix(Cr, 8);
}
