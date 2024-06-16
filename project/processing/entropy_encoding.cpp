// Block 8x8 -> {AC, DC} from zigzag
std::pair<int, std::vector<int>> zig_zag(Matrix<int> &block) {
    std::vector<int> sequence;
    _Bool direction_up = false;
    for (int diag = 1; diag < 15; ++diag) {
        for (int i = 0; i <= diag; ++i) {
            j = diag - i;
            if (i >= 0 && i < 8 && j >= 0 && j < 8) {
                if (direction_up) {
                    sequence.push_back(block[i][j]);

                } else {
                    sequence.push_back(block[j][i]);
                }
            }
        }
        direction_up = !direction_up;
    }
    return std::make_pair(block[0][0], sequence);
}

typedef struct {
    uint8_t RUNLENGTH; // Number of nulls before non-null current value
    uint8_t SIZE; // Size (in bits) of non-null current value
    int AMPLITUDE; // Quantized current-value itself
} current; // current is kept here too (for universal usage in Huffman). In that case RL = 0;

//typedef struct {
//    uint8_t SIZE; // Size (in bits) diffDC value
//    int AMPLITUDE; // Differencized current itself
//} current;

uint8_t bitsize(int n) {
    uint8_t size = 0;
    while (n) {
        ++size;
        n >>= 1;
    }
    return size;
}

// current sequence of one block -> RLE encoded with (0, 0) at the end. Don't forget that 15 nulls is maximum.
std::vector <current> RLE(std::vector<int> &sequence) {
    std::vector <current> encoded_sequence;
    int null_count = 0;
    for (auto k: sequence) {
        if (k == 0) {
            ++null_count;
            if (null == 15) {
                encoded_sequence.push_back({15, 0, 0});
                null_count = 0;
            }
        } else {
            encoded_sequence.push_back({null_count, bitsize(k), k});
            null_count = 0;
        }
    }
    encoded_sequence.push_back({0, 0, 0}); // Wiki says that here should be (0, 0)
    return encoded_sequence;
}

typedef struct {
    vector<current> DC_sequence;
    vector<vector<current>> AC_sequence;
} EncodedMatrix;

// Full matrix (Y/Cr/Cb) -> vector<int> DiffDC and vector<int> RLE(current) for each block
EncodedMatrix toACDC(Matrix<int> &M) {
    EncodedMatrix ACDC; // YEEEaaaHHH ROOcckkKKK
    int prevDC = 0;
    int n = M.getColumnSize();
    int m = M.getRowSize();
    for (int i = 0; i < n / 8; ++i) {
        for (int j = 0; j < m / 8; ++j) {
            auto block = M.getSlice({i * 8, j * 8},
                                    {i * 8 + 7, j * 8 + 7});
            auto blockACDC = zig_zag(block);
            ACDC.DC_sequence.push_back({0, bitsize(blockACDC.first - prevDC),
                                        blockACDC.first - prevDC}); // Add diffDC
            prevDC = blockACDC.first;
            ACDC.AC_sequence.emplace_back(RLE(blockACDC.second)); // Add RLE(vector<current>)
        }
    }
    return ACDC;
}

