
#include "TDL/Graphics/Display/Strategy/Ascii/AsciiMethode.hpp"

extern "C" {
    tdl::IDrawMethode *create(tdl::FrameBuffer &buffer) {
        return new tdl::AsciiMethode(buffer);
    }
}

extern "C" {
    void destroy(tdl::IDrawMethode *methode) {
        delete methode;
    }
}
