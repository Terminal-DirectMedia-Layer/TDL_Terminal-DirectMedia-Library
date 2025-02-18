#ifndef DISPLAY_HPP
    #define DISPLAY_HPP

#include "TDL/Graphics/FrameBuffer/FrameBuffer.hpp"
#include "TDL/Graphics/Display/Strategy/IDrawMethode.hpp"
#include "TDL/Graphics/Window/Window.hpp"
#include "TDL/Graphics/Drawable/Text/Font/Font.hpp"

#include "TDL/Event/EventNotifier.hpp"

#include "TDL/Graphics/Widget/Widget.hpp"

#include "TDL/Utils/Fps.hpp"
#include "TDL/Interrupt/InterruptManager.hpp"
#include "TDL/Utils/dynLibLoader.hpp"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

const std::string assetPath = TOSTRING(ASSETS_DIR);

namespace tdl
{
    typedef void (*eventDisplayCallback)(Event &event);

    enum class DisplayType
    {
        AUTO,
        TTY,
        SIXEL,
        ASCII
    };

    /**
     * @class Display
     * @brief A class that represents a display.
     */
    class Display : public FrameBuffer
    {
      public:

        static Display& getInstance(DisplayType type = DisplayType::AUTO) {
            static Display instance(type);
            return instance;
        }

        static void destroyInstance() {
            Display &instance = getInstance();
            instance.~Display();
        }

        Display(const Display &d) = delete;
        Display &operator=(const Display &d) = delete;

        /**
         * @brief Destructor for the Display class.
         */
        ~Display() {
            _eventNotifier.joinKeyboardThread();
            _eventNotifier.joinMouseThread();
            delete _drawMethode;
        }

        void addWindow(Window *win)
        {
            _windows.push_back(win);
        }

        std::vector<Window *> &getWindowsList()
        {
            return _windows;
        }

        Window *getWinFocus()
        {
            return _windows.back();
        }

        void close()
        {
            _open = false;
        }

        bool isOpen()
        {
            return _open;
        }

        EventNotifier &getNotifier() {
            return _eventNotifier;
        }

        void clear(Pixel background = Pixel(0, 0, 0, 255))
        {
            for (auto &win : _windows) {
                win->clearWin();
            }
            this->FrameBuffer::clear(background);
        }

        /**
         * @brief sets the draw methode of the display.
         * @param drawMethode The draw methode to set.
         */
        void setDrawMethode(IDrawMethode *drawMethode);

        /**
         * @brief gets the draw methode of the display.
         * @return The draw methode of the display.
         */
        IDrawMethode *getDrawMethode();

        void updateSize()
        {
            _drawMethode->updateSize(*this);
        }

        bool computePixel(Vector2i pos, Pixel & pixel);

        /**
         * @brief draws the display.
         */
        void draw();

        Font &getFont()
        {
            return _glFont;
        }

        /**
         * @brief norifyEvent
        */
        void pollEvent();
        eventDisplayCallback onEvent = nullptr;
        FPSCounter fps {};
        InterruptManager _interruptManager;
        Widget _cursor;

      protected:
          IDrawMethode *_drawMethode; /**< The draw methode of the display. */

      private :
            bool isGraphical(); /**< Check if the display is graphical. */
            bool supportsSixel(); /**< Check if the display supports Sixel. */
            DisplayType _type ; /**< The type of the display. */
            Vector2u _resolution; /**< The resolution of the display. */
            std::vector<Window *> _windows; /**< The windows of the display. */
            EventNotifier _eventNotifier; /**< The event notifier of the display. */
            DynLibLoader _libLoader; /**< The dynamic library loader of the display. */
            bool _open = true;

            Font _glFont;


        Display(DisplayType type = DisplayType::AUTO);


    };

}


#endif //DISPLAY_HPP
