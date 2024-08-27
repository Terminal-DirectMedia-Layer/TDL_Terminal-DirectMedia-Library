
#pragma once

#include "TDL/Event/Mouse/Imouse.hpp"
#include <iostream>
#include <string>

namespace tdl {
    class mouse : public Imouse {

        public:
            mouse();
            ~mouse() = default;

            bool mouseMove(AWindow  *window, EventMouseData data) override;
            bool mousePessed(AWindow  *window, EventMouseData data) override;
            bool mouseReleased(AWindow  *window, EventMouseData data) override;
            bool mouseScroll(AWindow  *window, EventMouseData data) override;
    };
}