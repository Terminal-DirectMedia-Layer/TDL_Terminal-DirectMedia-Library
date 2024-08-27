
#ifndef MOUSE_HPP
    #define MOUSE_HPP
#include "TDL/Event/Mouse/IMouse.hpp"
#include <iostream>
#include <string>

namespace tdl {
    class Mouse : public IMouse {

        public:
            Mouse();
            ~Mouse() = default;

            bool mouseMove(AWindow  *window, EventMouseData data) override;
            bool mousePessed(AWindow  *window, EventMouseData data) override;
            bool mouseReleased(AWindow  *window, EventMouseData data) override;
            bool mouseScroll(AWindow  *window, EventMouseData data) override;
    };
}

#endif // MOUSE_HPP