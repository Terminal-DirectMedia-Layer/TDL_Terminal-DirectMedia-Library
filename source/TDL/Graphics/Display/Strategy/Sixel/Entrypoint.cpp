
#include "TDL/Graphics/Display/Strategy/Sixel/SixelMethode.hpp"

extern "C" {
    tdl::IDrawMethode *create(tdl::FrameBuffer &buffer) {
        return new tdl::SixelMethode(buffer);
    }
}

extern "C" {
    void destroy(tdl::IDrawMethode *methode) {
        delete methode;
    }
}
