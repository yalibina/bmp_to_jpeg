#include "subsample.h"

void Subsample::chroma_subsample(Image &image) {
    for (uint32_t row = 0; row < image.GetImageHeight(); ++row) {
        if (row % 2 != 0) {
            continue;
        }
        for (uint32_t col = 0; col < image.GetImageWidth(); col += 2) {
            double sum_cb = 0;
            double sum_cr = 0;
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 2; ++j) {
                    sum_cb += image.At(std::min(row + i, image.GetImageHeight() - 1), std::min(col + j, image.GetImageWidth() - 1)).Cb;
                    sum_cr += image.At(std::min(row + i, image.GetImageHeight() - 1), std::min(col + j, image.GetImageWidth() - 1)).Cr;
                }
            }
            image.AtCb(row / 2, col / 2) = sum_cb / 4;
            image.AtCr(row / 2, col / 2) = sum_cr / 4;
        }
    }
}
