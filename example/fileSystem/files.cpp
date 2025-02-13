
#include "files.hpp"

std::ostream& operator<<(std::ostream& os, const File& file) {
    os << file._path;
    return os;
}