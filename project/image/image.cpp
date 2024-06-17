#include "image.h"

const uint8_t BITS_IN_BYTE = 8;
const uint8_t BMP_HEADER_SIZE = 54;

void Image::Resize(const uint32_t new_width, const uint32_t new_height) {
    image_width_ = std::min(image_width_, new_width);
    image_height_ = std::min(image_height_, new_height);

    pixel_matrix_.resize(image_height_, std::vector<Pixel>(image_width_));
    Y_matrix_.resize(image_width_, image_height_);
    Cb_matrix_.resize((image_width_ / 2) + (image_width_ % 2),
                      (image_height_ / 2) + (image_height_ % 2));
    Cr_matrix_.resize((image_width_ / 2) + (image_width_ % 2),
                      (image_height_ / 2) + (image_height_ % 2));
}

void Image::ReadHeader(std::ifstream &file_stream) {
    file_stream.read(reinterpret_cast<char *>(&bmp_file_header_), sizeof(bmp_file_header_));
    file_stream.read(reinterpret_cast<char *>(&bmp_info_header_), sizeof(bmp_info_header_));

    if (bmp_info_header_.bi_width < 0) {
        throw std::runtime_error("Image width cannot be negative.");
    }

    if (bmp_info_header_.bi_height < 0) {
        order_ = -1;
    }

    image_width_ = bmp_info_header_.bi_width;
    image_height_ = abs(bmp_info_header_.bi_height);
}

void Image::WriteHeader(std::ofstream &file_stream) {
    file_stream.write(reinterpret_cast<char *>(&bmp_file_header_), sizeof(bmp_file_header_));
    file_stream.write(reinterpret_cast<char *>(&bmp_info_header_), sizeof(bmp_info_header_));
}

const uint32_t Image::GetImageHeight() const {
    return image_height_;
}

const uint32_t Image::GetImageWidth() const {
    return image_width_;
}

void Image::SetImageSize(uint32_t width, uint32_t height) {
    uint8_t padding = GetPadding();
    bmp_info_header_.bi_size_image = height * (width * (bmp_info_header_.bi_bit_count / BITS_IN_BYTE) + padding);
    bmp_file_header_.bf_size = bmp_info_header_.bi_size_image + BMP_HEADER_SIZE;
}

Pixel &Image::At(uint32_t x, uint32_t y) {
    return pixel_matrix_[x][y];
}

double &Image::AtY(uint32_t x, uint32_t y) {
    return Y_matrix_(x, y);
}

double &Image::AtCb(uint32_t x, uint32_t y) {
    return Cb_matrix_(x,y);
}

double &Image::AtCr(uint32_t x, uint32_t y) {
    return Cr_matrix_(x,y);
}

const uint8_t Image::GetPadding() const {
    return ((4 - (bmp_info_header_.bi_width * (bmp_info_header_.bi_bit_count / BITS_IN_BYTE)) % 4)) % 4;
}

void Image::UpdateImageHeightInfo() {
    bmp_info_header_.bi_height = static_cast<int32_t>(image_height_) * order_;
}

void Image::UpdateImageWidthInfo() {
    bmp_info_header_.bi_width = image_width_;
}

const uint16_t Image::GetFileType() const {
    return bmp_file_header_.bf_type;
}

std::vector<std::vector<Pixel>> Image::GetPixelMatrix() const {
    return pixel_matrix_;
}

Matrix<double> Image::GetYMatrix() {
    return Y_matrix_;
}

Matrix<double> Image::GetCbMatrix() {
    return Cb_matrix_;
}

Matrix<double> Image::GetCrMatrix() {
    return Cr_matrix_;
}

const int Image::GetOrder() const {
    return order_;
}
