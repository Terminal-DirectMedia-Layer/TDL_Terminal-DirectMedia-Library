
#include "TDL/Graphics/Display/Strategy/TTY/TtyMethode.hpp"

extern "C" {
    tdl::IDrawMethode *create(tdl::FrameBuffer &buffer) {
        return new tdl::TtyMethode(buffer);
    }
}

extern "C" {
    void destroy(tdl::IDrawMethode *methode) {
        delete methode;
    }
}