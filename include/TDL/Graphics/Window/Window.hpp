
#ifndef TDL_WINDOW_HPP
    #define TDL_WINDOW_HPP

#include <string>
#include <list>
#include <iostream>
#include <array>
#include <vector>
#include <iostream>
#include <queue>
#include <regex>

#include <termios.h>

#include "TDL/Event/Event.hpp"
#include "TDL/Graphics/Drawable/ADrawable.hpp"
#include "TDL/Graphics/Drawable/Pixel/Pixel.hpp"
#include "TDL/Graphics/FrameBuffer/Feature/Placeable.hpp"
#include "TDL/Graphics/FrameBuffer/FrameBuffer.hpp"
#include "TDL/Utils/Math/Vector.hpp"
#include "TDL/Graphics/Drawable/Text/Text.hpp"

#include <TDL/Event/EventNotifier.hpp>

#define TDL_X_WINDOW_OFFSET 4
#define TDL_Y_WINDOW_OFFSET 17
#define TDL_WINDOW_BORDER_LEFT_B 2 // for the round up of the div
#define TDL_WINDOW_BORDER_RIGHT_B 2
#define TDL_WINDOW_BORDER_TOP_B 15
#define TDL_WINDOW_BORDER_BOTTOM_B 2


namespace tdl {
    typedef void (*eventCallback)(Event &event, Window *win);
    /**
     * @class Window
     * @brief Window class
     */

    class Display;


    enum ClickLocation {
        OUTSIDE,
        BORDER,
        INSIDE,
    };

    class Window : public FrameBuffer, public Placeable {
        public:

/**
 * \section Constructor / Destructor
 * 
 */

        Window(std::string title, Vector2u size,Vector2u pos = Vector2u(0,0), Pixel background = Pixel(18, 18, 18,255));

        /**
         * @brief Destroy the Window object
         * 
         */
            ~Window();

        void draw(Display &d);

        void pushEvent(const Event &event);
        void pollEvent();

        std::vector<IDrawable *> &getDrawables() { return _drawables; }

        inline ClickLocation isClickIn(Vector2u pos) {
            if (pos.x() >= _winPos.getPosition().x() && pos.x() <= _winPos.getPosition().x() + getSize().x()
                && pos.y() >= _winPos.getPosition().y() && pos.y() <= _winPos.getPosition().y() + getSize().y()) {
                if (pos.x() >= getPosition().x() && pos.x() <= getPosition().x() + getSize().x()
                    && pos.y() >= getPosition().y() && pos.y() <= getPosition().y() + getSize().y()) {
                    return INSIDE;
                }
                return BORDER;
            }
            return OUTSIDE;
        }

        bool grab = false;

        eventCallback onEvent = nullptr;

        void addDrawable(IDrawable *drawable) { _drawables.push_back(drawable); }

        Pixel &getBackground() {
            return _background;
        }

        bool getPixelAtPos(Vector2i pos, Pixel &pixel) {
            if (pos.x() < 0 || pos.y() < 0 || pos.x() >= getSize().x() || pos.y() >= getSize().y())
                return false;
            for (auto &drawable : _drawables) {
                if (drawable->getPixelAtPos(pos, pixel)) { //TODO crée une priorité sur les sprite pour les dessiner
                    return true;
                }
            }
            pixel = getPixel(pos.x(), pos.y());
            return true;
        }

        private:

/**
 * \section Attributes
 * 
 */
            //Keyboard* _input; /*!< the input keyboard */
            Vector2u _pos;
            Vector2u _size;
            Text _title;

            Pixel _background;

            FrameBuffer _win;
            Placeable _winPos;
            std::queue<Event> _events; /*!< the event queue */

            std::vector<IDrawable *> _drawables;

    };

}

#endif //TDL_WINDOW_HPP
