
#ifndef GPMMOUSE_HPP
    #define GPMMOUSE_HPP

#include "TDL/Event/Mouse/IMouse.hpp"
#include <iostream>
#include <string>
#include <gpm.h>

namespace tdl {
    class GpmMouse {

    public:
        GpmMouse();
        ~GpmMouse() = default;

        bool mouseMove(AWindow  *window) ;
        bool mousePessed(AWindow  *window) ;
        bool mouseReleased(AWindow  *window);
        bool mouseScroll(AWindow  *window);

        int getCharWidth(AWindow *window);
        int getCharHeight(AWindow *window);

    private:
        Gpm_Connect conn;
        Gpm_Event ev;
    };
}

#endif //GPMMOUSE_HPP
