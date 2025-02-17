
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
#include "TDL/Graphics/Window/Decorator.hpp"

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

    class Window : public FrameBuffer, public Placeable, public Decorator{
        public:

/**
 * \section Constructor / Destructor
 * 
 */

        Window(std::string title, Vector2u size,Vector2i pos = Vector2i(0,0), Pixel background = Pixel(18, 18, 18,255));

        /**
         * @brief Destroy the Window object
         * 
         */
            ~Window();

        void draw(Display &d);

        void pushEvent(const Event &event);
        void pollEvent();

        std::vector<IDrawable *> &getDrawables() { return _drawables; }

        inline bool isClickIn(Vector2i pos, ClickRegion region) {
            RectI decRect = getDimensions();
            if (pos.y() < decRect.y() || pos.y() > decRect.y() + decRect.height() || pos.x() < decRect.x() || pos.x() > decRect.x() + decRect.width()) {
                return ClickRegion::OUTSIDE == region;
            }
            if (pos.y() < getPosition().y() + TDL_WINDOW_BORDER_TOP_B) {
                return ClickRegion::TITLE_BAR == region;
            }
            if (pos.y() >= getPosition().y() + getSize().y() && pos.y() <= decRect.height() + decRect.y()) {
                return ClickRegion::RESIZE_BOTOM == region;
            }
            if (pos.x() >= getPosition().x()  + getSize().x() && pos.x() <= decRect.width() + decRect.x()) {
                return ClickRegion::RESIZE_RIGHT == region;
            }
            if (pos.x() < getPosition().x() && pos.x() > decRect.x()) {
                return ClickRegion::RESIZE_LEFT == region;
            }
            if (pos.y() > getPosition().y() && pos.y() < getSize().y() + getPosition().y() && pos.x() > getPosition().x() && pos.x() < getSize().x() + getPosition().x()) {
                return ClickRegion::INSIDE == region;
            }
            return ClickRegion::NONE == region;
        }

        bool grab = false;
        ClickRegion region = ClickRegion::NONE;

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

        void resizeWindow(Vector2u size) {
            setDimensions(RectI(getPosition().x() - TDL_WINDOW_BORDER_LEFT_B, getPosition().y() - TDL_WINDOW_BORDER_TOP_B, size.x() + TDL_WINDOW_BORDER_LEFT_B + TDL_WINDOW_BORDER_RIGHT_B, size.y() + TDL_WINDOW_BORDER_TOP_B + TDL_WINDOW_BORDER_BOTTOM_B));
            resize(size);
            _size = size;
        }

        void clearWin() {
            if (isPosChanged()) {
                setPosition(_pos);
            }
            if (isSizeChanged()) {
                resizeWindow(_size);
            }
            clear(_background);
        }

        void setWinSize(Vector2u size) {
            _size = size;
            sizeChanged = true;
        }

        void setWinPos(Vector2i pos) {
            _pos = pos;
            posChanged = true;
        }

        bool isSizeChanged() {
            if (sizeChanged) {
                sizeChanged = false;
                return true;
            }
            return false;
        }

        bool isPosChanged() {
            if (posChanged) {
                posChanged = false;
                return true;
            }
            return false;
        }

        private:

/**
 * \section Attributes
 * 
 */
            //Keyboard* _input; /*!< the input keyboard */
            Vector2i _pos;
            Vector2u _size;
            Text _title;

            Pixel _background;

            std::queue<Event> _events; /*!< the event queue */

            std::vector<IDrawable *> _drawables;

            bool sizeChanged = false;
            bool posChanged = false;

    };

}

#endif //TDL_WINDOW_HPP
