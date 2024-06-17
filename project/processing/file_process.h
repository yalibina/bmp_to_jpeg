#pragma once

#include "../image/image.h"
#include "subsample.h"
#include "quantization.h"

#include <fstream>

class File {
public:
    Image ReadFile(const std::string& filename);
    static void WriteFile(const std::string& filename, Image& image, int q);
};
