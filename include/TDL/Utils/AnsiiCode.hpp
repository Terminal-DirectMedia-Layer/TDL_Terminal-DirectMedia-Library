
#ifndef TDL_ANSII_CODE_HPP
    #define TDL_ANSII_CODE_HPP

#include <string>
#include <regex>

#include <unistd.h>


#include "TDL/Graphics/Drawable/Pixel/Pixel.hpp"
#include "TDL/Utils/Math/Vector.hpp"

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

            /**
            * @brief anssi start sixel code
            */
            inline void startSixel()
            {
                _content += "\033P0;0;0;q";
            }

            inline void endSixel()
            {
                _content += "\033\\";
            }

            inline void setSixelColor(int id, Pixel pixel, bool isBack = false)
            {
              if (isBack){
                _content += '#';
                _content += std::to_string(id).c_str();
              }else {
            	_content += '#';
                _content += std::to_string(id).c_str();
                _content += ";2;";
                _content += std::to_string(GET_R(pixel.color) * 100 / 255).c_str();
                _content += ';';
                _content += std::to_string(GET_G(pixel.color) * 100 / 255).c_str();
                _content += ';';
                _content += std::to_string(GET_B(pixel.color) * 100 / 255).c_str();
                }
            }

            inline void jumpSixelColumn()
            {
                _content += '$';
            }

            inline void jumpSixelLine()
            {
                _content += '-';
            }

            void turnSixelOn(std::string mask)
            {
                _content += mask.c_str();
            }

            inline Vector2u getSixelSize(int fd)
            {
                write(fd, "\033[14t", 5);
                char buf[32];
                read(fd, buf, 32);
                std::regex re(R"((\d+);(\d+);(\d+)*)");
                std::smatch match;
                std::string str(buf);

                if (std::regex_search(str, match, re)) {
                    return Vector2u(std::stoi(match[3]), std::stoi(match[2]));
                }
                return Vector2u(0, 0);
            }

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