
#pragma once

#include "tdl/Event/Mouse/Imouse.hpp"
#include <iostream>
#include <string>

namespace tdl {
    class mouse : public Imouse {

        public:
            mouse();
            ~mouse() = default;

            bool mouseMove(Window  *window, EventMouseData data) override;
            bool mousePessed(Window  *window, EventMouseData data) override;
            bool mouseReleased(Window  *window, EventMouseData data) override;
            bool mouseScroll(Window  *window, EventMouseData data) override;
    };
}