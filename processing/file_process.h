#ifndef CPP_HSE_FILE_PROCESS_H
#define CPP_HSE_FILE_PROCESS_H

#include "../image/image.h"

#include <fstream>

class File {
public:
    Image ReadFile(const std::string& filename);
    void WriteFile(const std::string& filename, Image& image);
};

#endif  // CPP_HSE_FILE_PROCESS_H
