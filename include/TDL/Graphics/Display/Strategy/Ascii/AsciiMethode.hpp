/**
* @file AsciiMethode.hpp
* @brief A file that contains the ASCII draw methode.
*/
#ifndef ASCIIMETHODE_HPP
    #define ASCIIMETHODE_HPP

#include "TDL/Graphics/Display/Strategy/IDrawMethode.hpp"
#include "TDL/Utils/AnsiiCode.hpp"

#include <termios.h>

namespace tdl
{

   /**
    * @struct CharColor
    * @brief the CharColor structure
    * it permited to store the color of a char
    * and the shape of the char
    * @note the shape is a string of 6 char
    */
   struct CharColor {
       const char *shape{};
       Pixel ForeGround;
       Pixel BackGround;
   };

    /**
     * @class AsciiMethode
     * @brief A class that represents the ASCII draw methode.
     */
    class AsciiMethode : public IDrawMethode, public AnsiiCode
    {
      public:

        /**
         * @brief Constructor for the AsciiMethode class.
         */
        AsciiMethode(FrameBuffer &buffer);

        /**
         * @brief Destructor for the AsciiMethode class.
         */
        ~AsciiMethode() {}

        /**
         * @brief draws the display.
         * @param display The display to draw.
         */
        void draw(FrameBuffer &buffer) override;

        void updateSize(FrameBuffer &buffer) override;

        private:
          bool getPixelChar(u_int32_t x, u_int32_t y, Pixel *pixel, FrameBuffer &buffer);
          void setPixelChar(u_int32_t x, u_int32_t y, Pixel *pixel, FrameBuffer &buffer);
          CharColor computeCharColor(Vector2u pos, Pixel *pixels, CharColor &charColor);

          struct termios _tty{}; /* !< the terminal configuration */
          int _fd{}; /*!< the fd of the window */



    };
}

#endif //ASCIIMETHODE_HPP
