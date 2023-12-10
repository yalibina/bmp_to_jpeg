//
// Created by libin on 19.03.2023.
//

#ifndef CPP_HSE_PIXEL_H
#define CPP_HSE_PIXEL_H

#include <cstdint>

class Pixel {
public:
    double R = 0;
    double G = 0;
    double B = 0;

    Pixel operator+(const Pixel& pxl) const;
    Pixel operator+=(const Pixel& pxl);

    Pixel operator*(const double val) const;
    Pixel operator*=(const double val);

    Pixel& Norm(double threshold = -1);
};

#endif  // CPP_HSE_PIXEL_H
