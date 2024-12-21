
#ifndef TDL_ANSII_CODE_HPP
    #define TDL_ANSII_CODE_HPP

#include <string>

#include "TDL/Pixel/Pixel.hpp"
#include "TDL/Math/Vector.hpp"

namespace tdl {
    /**
     * @class AnsiiCode
     * @brief AnsiiCode class
     */
    class AnsiiCode {
        
        protected:

        /**
         * @brief ansii front color code
         * 
         * @param pixel the pixel to get the color
         */
            inline void setRGBFrontGround(Pixel pixel)
            {
                _content += "\033[38;2;" + std::to_string(GET_R(pixel.color)) + ";" + std::to_string(GET_G(pixel.color)) + ";" + std::to_string(GET_B(pixel.color)) + "m";
            }

        /**
         * @brief ansii back color code
         * 
         * @param pixel the pixel to get the color
         */
            inline void setRGBBackGround(Pixel pixel)
            {
                _content += "\033[48;2;" + std::to_string(GET_R(pixel.color)) + ";" + std::to_string(GET_G(pixel.color)) + ";" + std::to_string(GET_B(pixel.color)) + "m";
            }

        /**
         * @brief ansii clear screen code
         * 
         */
            inline void clearScreen()
            {
                _content += "\033[2J";
            }
 
        /**
         * @brief ansii move cursor code
         * 
         * @param pos the position to move the cursor
         */
            inline void moveCursor(Vector2u pos)
            {
                _content += "\033[" + std::to_string(pos.y() / 2) + ";" + std::to_string(pos.x()) + "H";
            }

        /**
         * @brief ansii print pixel code
         * 
         */
            inline void printPixel(const char *shape)
            {
                if (shape == nullptr)
                    return;
                _content += std::string(shape);
            }

        /**
         * @brief ansii alternate screen buffer code
         * 
         */   
            inline void alternateScreenBuffer()
            {
                _content += "\033[?1049h";
            }

        /**
         * @brief ansii remove mouse cursor code
         * 
         */
            inline void removeMouseCursor()
            {
                _content += "\033[?25l";
            }

        /**
         * @brief ansii enable mouse move code
         * 
         */
            inline void enableMouseMove()
            {
                _content += "\033[?1003h";
                _content += "\033[?1006h";
            }

        /**
         * @brief ansii disable mouse move code
         * 
         */
            inline void disableMouseMove()
            {
                _content += "\033[?1003l";
                _content += "\033[?1006l";
            }

        /**
         * @brief ansii enable mouse click code
         * 
         */
            inline void enableMouseClick()
            {
                _content += "\033[?1000h";
            }

        /**
         * @brief ansii disable mouse click code
         * 
         */
            inline void disableMouseClick()
            {
                _content += "\033[?1000l";
            }

            virtual void draw() = 0;

        /**
         * @brief get the content of the window
         * 
         * @return std::string the content of the window
         */
            [[nodiscard]] std::string &getContent() { return _content; }


        std::string _content; /* !< the content of the window */

    };
}

#endif //TDL_ANSII_CODE_HPP