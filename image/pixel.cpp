#include <algorithm>
#include "pixel.h"

Pixel Pixel::operator+(const Pixel& pxl) const {
    Pixel new_pixel = Pixel({0, 0, 0});
    new_pixel.R = this->R + pxl.R;
    new_pixel.G = this->G + pxl.G;
    new_pixel.B = this->B + pxl.B;
    return new_pixel;
}
Pixel Pixel::operator+=(const Pixel& pxl) {
    *this = *this + pxl;
    return *this;
}
Pixel Pixel::operator*(const double val) const {
    Pixel new_pixel = Pixel({0, 0, 0});
    new_pixel.R = this->R * val;
    new_pixel.G = this->G * val;
    new_pixel.B = this->B * val;
    return new_pixel;
}

Pixel Pixel::operator*=(const double val) {
    *this = *this * val;
    return *this;
}

Pixel& Pixel::Norm(double threshold) {
    this->R = std::max(0., (std::min(1., this->R)));
    this->G = std::max(0., (std::min(1., this->G)));
    this->B = std::max(0., (std::min(1., this->B)));

    // for Edge filter only, consider all colors to have the same values
    if (threshold != -1) {
        if (this->R > threshold) {
            *this = {1, 1, 1};
        } else {
            *this = {0, 0, 0};
        }
    }

    return *this;
}
