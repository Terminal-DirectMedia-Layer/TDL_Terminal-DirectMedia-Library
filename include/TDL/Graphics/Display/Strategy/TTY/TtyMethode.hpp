/**
* @file TtyMethode.hpp
* @brief A file that contains the TTY draw methode.
*/
#ifndef TTYMETHODE_HPP
#define TTYMETHODE_HPP

#include "TDL/Graphics/Display/Strategy/IDrawMethode.hpp"

#include <linux/fb.h>
#include <sys/ioctl.h>

#include <memory>


namespace tdl {

        /**
        * @class TtyMethode
        * @brief A class that represents the TTY draw methode.
        */
        class TtyMethode : public IDrawMethode
        {
        public:

            /**
            * @brief Constructor for the TtyMethode class.
            */
            TtyMethode(FrameBuffer &buffer);

            /**
            * @brief Destructor for the TtyMethode class.
            */
            ~TtyMethode();

            /**
            * @brief draws the display.
            * @param display The display to draw.
            */
            void draw(FrameBuffer &buffer) override;

            void updateSize(FrameBuffer &buffer) override {}

            private:
             	int _fd; /**< The file descriptor of the TTY. */
                struct fb_var_screeninfo _vinfo; /**< The variable information of the framebuffer. */
                struct fb_fix_screeninfo _finfo; /**< The fixed information of the framebuffer. */
                long int _screensize; /*!< the size of the screen in bytes */
                char *_fbp; /*!< the framebuffer pointer */
                std::unique_ptr<char[]> _startScreen; /*!< the screen at the start of the program */
                Vector2u _size; /*!< the terminal size */
        };
    }

#endif //TTYMETHODE_HPP
