#ifndef AWINDOW_HPP
    #define AWINDOW_HPP

#include <string>
#include <list>
#include <iostream>
#include <array>
#include <vector>
#include <termios.h>
#include <iostream>
#include <queue>
#include <regex>
#include "tdl/Pixel/Pixel.hpp"
#include "tdl/Vector.hpp"
#include "tdl/Matrix/PixelMatrix.hpp"
#include "tdl/Drawable/Drawable.hpp"
#include "tdl/Event/Event.hpp"
#include "tdl/Input/inputKeyboard.hpp"
#include "tdl/Event/Mouse/Imouse.hpp"
#include "tdl/Window/ansiiCode.hpp"

namespace tdl {
    /**
     * @class AWindow
     * @brief AWindow anstract class
     */

    class AWindow : public Drawable, public Event, public ansiiCode {
        
        public:

/**
 * @brief Create a AWindow object
 * 
 */
            AWindow() = default;

/**
 * @brief destroy the AWindow object
 * 
 */
            virtual ~AWindow() = default;

        /**
         * @brief clear the pixel matrix
         * 
         */
            void clearPixel();

        /**
         * @brief update the window with the new pixel to draw on the screen by generating an optimized ansii escape code sequence
         * 
         * @param all an boolean to force the update of all the pixel
         * @note if all is true the optimisation will be skip and all the screen is generated it will be useful at the start of an window or if you want to clear it to black
         */
            void update(bool all = false);

        /**
         * @brief Get the Height object
         * 
         * @return u_int32_t the height of the window
         */
            [[nodiscard]] u_int32_t getHeight() { return _size.y(); }

        /**
         * @brief Get the Width object
         * 
         * @return u_int32_t the width of the window
         */
            [[nodiscard]] u_int32_t getWidth() { return _size.x(); }

        /**
         * @brief Get the Frame Rate object
         * 
         * @return u_int8_t the frame rate of the window
         */
            [[nodiscard]] u_int8_t getFrameRate() const { return _frameRate; }

        /**
         * @brief Get the Fd object
         * 
         * @return int the fd of the window
         */
            [[nodiscard]] int getFd() const { return _fd; }

        /**
         * @brief Set the Frame Rate object
         * 
         * @param frameRate the new frame rate
         */
            void setFrameRate(u_int8_t frameRate) { _frameRate = frameRate; }


        /**
         * @brief poll the event
         * 
         * @param event the event to poll
         * @return true if event is left in the queue
         * @return false if no event is left in the queue
         */
        virtual bool pollEvent(Event &event, std::regex *custom = nullptr) = 0;

        /**
         * @brief update the terminal size
         * @warning this function is called at the creation of the window and when the terminal size change
         * by the signale handler 
         */
            virtual void updateTermSize() = 0;

        /**
         * @brief print the frame rate on the terminal
         * 
         */
            void printFrameRate();

        /**
         * @brief write the content generate by updatePixel to the terminal
         * and save the current pixel to the old pixel table
         * @note the copy of the pixel in the oldpixel is make two at a time because 1 charactere = 2 pixel
         */
            void draw() override;

        protected:

        /**
         * @brief disable the echo of the terminal
         * we disable the echo of the terminal to avoid the input to be displayed on the terminal
         */
            void disableEcho();

        void draw(Drawable *drawable) { return; } /* actualy does nothing an ti does any vocation to be call*/


            int _fd{}; /*!< the fd of the window */
            std::string _title; /* !< the title of the window */
            u_int8_t _frameRate = 60; /* !< the frame rate of the window */
            Vector2u _size; /* !< the size of the window */
            struct termios _tty{}; /* !< the terminal configuration */
            bool _update = true; /* !< a boolean to check if the window need to be updated */
            int framecounter = 0; /* !< the frame counter */
            std::chrono::time_point<std::chrono::system_clock> start; /* !< the start time of the frame */

    };
}

#endif //AWINDOW_HPP