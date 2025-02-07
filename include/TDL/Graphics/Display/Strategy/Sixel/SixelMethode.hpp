// File: SixelMethode.hpp
#ifndef SIXELMETHODE_HPP
    #define SIXELMETHODE_HPP

#include "TDL/Graphics/Display/Strategy/IDrawMethode.hpp"
#include "TDL/Utils/AnsiiCode.hpp"
#include "TDL/Utils/Math/Vector.hpp"

#include <unordered_map>
#include <list>

#include <termios.h>
#include <sixel.h>

namespace tdl
{
    class Sixel {
        public:
        Sixel(int x, int y, FrameBuffer &buffer) {

            if (x >= buffer.getSize().x() || y >= buffer.getSize().y() - 6) {
                throw std::runtime_error("Error: x or y out of range");
            }
            for (int i = 0; i < 6; i++) {
              	Pixel s = buffer.getPixel(x, y + i);
                _sixel[i] = Pixel((GET_R(s.color) * 100) / 255, (GET_G(s.color) * 100) / 255, (GET_B(s.color) * 100) / 255, 0);
            }
        }
        ~Sixel() {}
        Pixel _sixel[6];
    };

    /**
     * @class AsciiMethode
     * @brief A class that represents the ASCII draw methode.
     */
    class SixelMethode : public IDrawMethode, public AnsiiCode
    {
    public:

        /**
         * @brief Constructor for the AsciiMethode class.
         */
        SixelMethode(FrameBuffer &buffer);

        /**
         * @brief Destructor for the AsciiMethode class.
         */
        ~SixelMethode() {}

        /**
         * @brief draws the display.
         * @param display The display to draw.
         */
        void draw(FrameBuffer &buffer) override;

        void updateSize(FrameBuffer &buffer) override;

        FrameBuffer atkinsonDithering(FrameBuffer buff);
        void generatePalette();
        Pixel findClosestColor(Pixel pixel);

    private:

        struct termios _tty{}; /* !< the terminal configuration */
        int _fd{}; /*!< the fd of the window */

        int _colorIndex = 1;
        std::unordered_map<u_int32_t, int> _colorMap;
        std::unordered_map<u_int32_t, std::string> _mask;
        std::vector<Pixel> _colorPalette;
        int _maskIndex = 0;

        sixel_output_t *_output;
        sixel_dither_t *_dither;

        void transform(Sixel &pixel);



    };
}


#endif //SIXELMETHODE_HPP
