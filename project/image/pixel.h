#pragma once
#include <cstdint>

class Pixel {
public:
    double R = 0;
    double G = 0;
    double B = 0;
    double Y = 0;
    double Cb = 0;
    double Cr = 0;

    Pixel operator+(const Pixel& pxl) const;
    Pixel operator+=(const Pixel& pxl);

    Pixel operator*(const double val) const;
    Pixel operator*=(const double val);

};

