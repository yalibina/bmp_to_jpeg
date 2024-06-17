#include "file_process.h"
#include "discrete_cosine_transform.h"
#include "quantization.h"
#include "entropy_encoding.h"
#include <cstring>

#include <stdexcept>

const uint16_t LITTLE_ENDIAN_TYPE = 0x4D42;
const uint16_t BIG_ENDIAN_TYPE = 0x424D;

Image File::ReadFile(const std::string& filename) {
    Image image;
    std::ifstream file_stream(filename, std::ifstream::binary);

    if (!file_stream) {
        std::cerr << "Error: " << strerror(errno);
        exit(1);
    }

    image.ReadHeader(file_stream);

    if (image.GetFileType() != LITTLE_ENDIAN_TYPE && image.GetFileType() != BIG_ENDIAN_TYPE) {
        throw std::runtime_error("File is not a BMP.");
    }

    uint8_t padding = image.GetPadding();

    // Read Pixels

    image.Resize(image.GetImageWidth(), image.GetImageHeight());

    for (uint32_t i = 0; i < image.GetImageHeight(); ++i) {

        uint32_t row = image.GetImageHeight() - i - 1;
        if (image.GetOrder() < 0) {
            row = i;
        }

        for (uint32_t j = 0; j < image.GetImageWidth(); ++j) {
            uint8_t red = 0;
            uint8_t green = 0;
            uint8_t blue = 0;

            file_stream.read(reinterpret_cast<char*>(&blue), 1);
            file_stream.read(reinterpret_cast<char*>(&green), 1);
            file_stream.read(reinterpret_cast<char*>(&red), 1);

            image.At(row, j).R = static_cast<double>(red);
            image.At(row, j).G = static_cast<double>(green);
            image.At(row, j).B = static_cast<double>(blue);

            double Y = 0.299*image.At(row, j).R + 0.587*image.At(row, j).G + 0.114*image.At(row, j).B;
            double Cb = 128 - 0.168736*image.At(row, j).R - 0.331264*image.At(row, j).G + 0.5*image.At(row, j).B;
            double Cr = 128 + 0.5*image.At(row, j).R - 0.418688*image.At(row, j).G  - 0.081312 *image.At(row, j).B;
            image.At(row, j).Y = Y;
            image.AtY(row, j) = Y;
            image.At(row, j).Cb = Cb;
            image.At(row, j).Cr = Cr;
        }
        file_stream.seekg(padding, std::ios_base::cur);
    }
    return image;
}

// helper function to write a 2-byte short integer in big-endian
void put_short(std::ofstream& file_stream, const uint v) {
    file_stream.put((v >> 8) & 0xFF);
    file_stream.put((v >> 0) & 0xFF);
}

void DHT(std::ofstream& file_stream, byte acdc, byte tableID, const HuffmanTable& htable) {
    file_stream.put(0xFF);
    file_stream.put(0xC4);
    byte code_amount = 0;
    for (auto c : htable.codelengths) {
        code_amount += c;
    }
    put_short(file_stream, 19 + code_amount);
    file_stream.put(acdc << 4 | tableID);
    for (uint i = 0; i < 16; ++i) {
        file_stream.put(htable.codelengths[i]);
    }
    for (uint i = 0; i < code_amount; ++i) {
        file_stream.put(htable.symbols[i]);
    }
}

void write_block(std::ofstream& file_stream,
                 EncodedMatrix &Enc_M,
                 size_t BH, size_t BW,
                 int i, int j) {

    file_stream.put(Enc_M.DC_sequence[i * BW + j].ENC_RLS);
    file_stream.put(Enc_M.DC_sequence[i * BW + j].AMPLITUDE);

    for (auto CURRENT : Enc_M.AC_sequence[i * BW +j]) {
        file_stream.put(CURRENT.ENC_RLS);
        file_stream.put(CURRENT.AMPLITUDE);
    }
};

void File::WriteFile(const std::string& filename, Image& image, int q) {
    std::ofstream file_stream(filename, std::ofstream::binary);

    if (!file_stream) {
        throw std::runtime_error("Could not open output file.");
    }

    Subsample::chroma_subsample(image);

    Matrix<double> Y = image.GetYMatrix();
    Matrix<double> Cb = image.GetCbMatrix();
    Matrix<double> Cr = image.GetCrMatrix();

    std::cout << "Got matrices\n";

    auto SIZE_OF_BLOCK = 8;

    Matrix<double> DCT_Y = DCT::applyDiscreteCosineTransform(Y, 8);
    Matrix<double> DCT_Cb = DCT::applyDiscreteCosineTransform(Cb, 8);
    Matrix<double> DCT_Cr = DCT::applyDiscreteCosineTransform(Cr, 8);
    std::cout << "DCT\n";

    auto IMAGE_HEIGHT = DCT_Y.getColumnSize();
    auto IMAGE_WIDTH = DCT_Y.getRowSize();
    std::cout << "After DCT: " << IMAGE_HEIGHT << ' ' << IMAGE_WIDTH << '\n';


    Matrix<int> Quant_Y = Quant::applyQuantization(DCT_Y, 8, 95);
    std::cout << "kek\n";
    Matrix<int> Quant_Cb = Quant::applyQuantization(DCT_Cb, 8, q);
    Matrix<int> Quant_Cr = Quant::applyQuantization(DCT_Cr, 8, q);
    std::cout << "Quant\n";

    EncodedMatrix Enc_Y = Entropy::encode_matrix(Quant_Y, 8);
    EncodedMatrix Enc_Cb = Entropy::encode_matrix(Quant_Cb, 8);
    EncodedMatrix Enc_Cr = Entropy::encode_matrix(Quant_Cr, 8);
    std::cout << "Encode\n";

    // SOI
    file_stream.put(0xFF);
    file_stream.put(0xD8);
    
    // APP0
    file_stream.put(0xFF);
    file_stream.put(0xE0);
    put_short(file_stream, 16);
    file_stream.put('J');
    file_stream.put('F');
    file_stream.put('I');
    file_stream.put('F');
    file_stream.put(0);
    file_stream.put(1);
    file_stream.put(2);
    file_stream.put(0);
    put_short(file_stream, 100); // хз че такое
    put_short(file_stream, 100);
    file_stream.put(0);
    file_stream.put(0);

    // DQT
    file_stream.put(0xFF);
    file_stream.put(0xDB);
    put_short(file_stream, 67);
    file_stream.put(0); // TableID
    auto QM = Quant::getQMatrix(8, q);
    for (uint i = 0; i < 8; ++i) {
        for (uint j = 0; j < 8; ++j) {
            file_stream.put(QM(i, j));
        }
    }

    // SOF0
    file_stream.put(0xFF);
    file_stream.put(0xC0);
    file_stream.put(17);
    file_stream.put(8);
    put_short(file_stream, IMAGE_HEIGHT);
    put_short(file_stream, IMAGE_WIDTH);
    file_stream.put(3);
    for (uint i = 1; i <= 3; ++i) {
        file_stream.put(i);
        file_stream.put(0x11);
        file_stream.put(i == 1 ? 0 : 1);
    }

    // DHT
    DHT(file_stream, 0, 0, Enc_Y.DC_table); // DC - 0; AC - 1;
    DHT(file_stream, 0, 1, Enc_Cb.DC_table);
    DHT(file_stream, 0, 2, Enc_Cr.DC_table);
    DHT(file_stream, 1, 0, Enc_Y.AC_table);
    DHT(file_stream, 1, 1, Enc_Cb.AC_table);
    DHT(file_stream, 1, 2, Enc_Cr.AC_table);

    // SOS
    file_stream.put(0xFF);
    file_stream.put(0xDA);
    put_short(file_stream, 12);
    file_stream.put( 3);
    for (uint i = 1; i <= 3; ++i) {
        file_stream.put(i);
        file_stream.put(i == 1 ? 0x00 : 0x11);
    }
    file_stream.put(0);
    file_stream.put(63); //?
    file_stream.put(0);

    // ECS
    auto BH = IMAGE_HEIGHT / SIZE_OF_BLOCK;
    auto BW = IMAGE_WIDTH / SIZE_OF_BLOCK;
    for (int i = 0; i < BH; i += 2) {
        for (int j = 0; j < BW; j += 2) {
            write_block(file_stream, Enc_Y, BH, BW, i, j);
            write_block(file_stream, Enc_Y, BH, BW, i, j + 1);
            write_block(file_stream, Enc_Y, BH, BW, i + 1, j);
            write_block(file_stream, Enc_Y, BH, BW, i + 1, j + 1);

            write_block(file_stream, Enc_Cb, BH / 2, BW / 2, i / 2, j / 2);
            write_block(file_stream, Enc_Cr, BH / 2, BW / 2, i / 2, j / 2);
        }
    }

    // EOI
    file_stream.put(0xFF);
    file_stream.put(0xD9);
    
    file_stream.close();
}