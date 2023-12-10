#include "../image/image.h"

#include <fstream>

class File {
public:
    Image ReadFile(const std::string& filename);
};
