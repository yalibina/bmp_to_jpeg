#include "file_process.h"

#include <stdexcept>

const uint16_t LITTLE_ENDIAN_TYPE = 0x4D42;
const uint16_t BIG_ENDIAN_TYPE = 0x424D;
const uint8_t NORM_BYTE = 255;

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

            image.At(row, j).R = static_cast<double>(red) / NORM_BYTE;
            image.At(row, j).G = static_cast<double>(green) / NORM_BYTE;
            image.At(row, j).B = static_cast<double>(blue) / NORM_BYTE;
        }
        file_stream.seekg(padding, std::ios_base::cur);
    }

    return image;
}

void File::WriteFile(const std::string& filename, Image& image) {
    std::ofstream file_stream(filename, std::ofstream::binary);

    if (!file_stream) {
        throw std::runtime_error("Could not open output file.");
    }

    image.UpdateImageHeightInfo();
    image.UpdateImageWidthInfo();

    uint8_t padding = image.GetPadding();

    image.SetImageSize(image.GetImageWidth(), image.GetImageHeight());

    image.WriteHeader(file_stream);

    // Write pixels

    uint32_t zero = 0;

    for (uint32_t i = 0; i < image.GetImageHeight(); ++i) {

        uint32_t row = image.GetImageHeight() - i - 1;
        if (image.GetOrder() < 0) {
            row = i;
        }

        for (uint32_t j = 0; j < image.GetImageWidth(); ++j) {
            uint8_t red = static_cast<uint8_t>(image.At(row, j).R * NORM_BYTE);
            uint8_t green = static_cast<uint8_t>(image.At(row, j).G * NORM_BYTE);
            uint8_t blue = static_cast<uint8_t>(image.At(row, j).B * NORM_BYTE);

            file_stream.write(reinterpret_cast<char*>(&blue), 1);
            file_stream.write(reinterpret_cast<char*>(&green), 1);
            file_stream.write(reinterpret_cast<char*>(&red), 1);
        }
        file_stream.write(reinterpret_cast<char*>(&zero), padding);
    }
}
