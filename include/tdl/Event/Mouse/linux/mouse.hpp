
#pragma once

#include "tdl/Window/windowBase.hpp"
#include "tdl/Event/Mouse/Imouse.hpp"
#include <linux/input.h>
#include <iostream>
#include <string>

namespace tdl {
    class mouse : public Imouse {

        public:
            mouse();
            ~mouse() = default;

            bool mouseMove(WindowBase  *window, EventMouseData data) override;
            bool mousePessed(WindowBase  *window, EventMouseData data) override;
            bool mouseReleased(WindowBase  *window, EventMouseData data) override;
            bool mouseScroll(WindowBase  *window, EventMouseData data) override;
    };
}