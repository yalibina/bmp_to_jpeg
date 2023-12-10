#ifndef CPP_HSE_IMAGE_H
#define CPP_HSE_IMAGE_H

#include <vector>
#include <fstream>

#include "pixel.h"

struct BMPFileHeader {
    uint16_t bf_type;
    uint32_t bf_size;
    uint16_t bf_reserved1;
    uint16_t bf_reserved2;
    uint32_t bf_off_bits;
} __attribute__((packed));

struct BMPInfoHeader {
    uint32_t bi_size;
    uint32_t bi_width;
    int32_t bi_height;
    uint16_t bi_planes;
    uint16_t bi_bit_count;
    uint32_t bi_compression;
    uint32_t bi_size_image;
    uint32_t bi_x_pels_per_meter;
    uint32_t bi_y_pels_per_meter;
    uint32_t bi_clr_used;
    uint32_t bi_clr_important;
} __attribute__((packed));

class Image {
public:
    void Resize(const uint32_t new_width, const uint32_t new_height);

    void ReadHeader(std::ifstream& fp);
    void WriteHeader(std::ofstream& fp);

    Pixel& At(uint32_t x, uint32_t y);

    const uint32_t GetImageHeight() const;
    const uint32_t GetImageWidth() const;
    const uint8_t GetPadding() const;
    const uint16_t GetFileType() const;
    const int GetOrder() const;
    std::vector<std::vector<Pixel>> GetPixelMatrix() const;

    void SetImageSize(uint32_t width, uint32_t height);
    void UpdateImageHeightInfo();
    void UpdateImageWidthInfo();

protected:
    uint32_t image_height_ = 0;
    uint32_t image_width_ = 0;
    int order_ = 1;

    std::vector<std::vector<Pixel>> pixel_matrix_;

    BMPFileHeader bmp_file_header_;
    BMPInfoHeader bmp_info_header_;
};

#endif  // CPP_HSE_IMAGE_H
