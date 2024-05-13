#include "file_process.h"

#include <stdexcept>

const uint16_t LITTLE_ENDIAN_TYPE = 0x4D42;
const uint16_t BIG_ENDIAN_TYPE = 0x424D;

Image File::ReadFile(const std::string& filename) {
    Image image;
    std::ifstream file_stream(filename, std::ifstream::binary);

    if (!file_stream) {
        throw std::runtime_error("Could not open input file.");
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

