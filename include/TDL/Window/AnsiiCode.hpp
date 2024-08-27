
#ifndef TDL_ANSII_CODE_HPP
    #define TDL_ANSII_CODE_HPP

#include <string>

#include "TDL/Pixel/Pixel.hpp"
#include "TDL/Vector.hpp"

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
            void setRGBFrontGround(Pixel color);

        /**
         * @brief ansii back color code
         * 
         * @param pixel the pixel to get the color
         */
            void setRGBBackGround(Pixel color);

        /**
         * @brief ansii clear screen code
         * 
         */
            void clearScreen();
 
        /**
         * @brief ansii move cursor code
         * 
         * @param pos the position to move the cursor
         */
            void moveCursor(Vector2u pos);

        /**
         * @brief ansii print pixel code
         * 
         */
            void printPixel(const char *shape);

        /**
         * @brief ansii alternate screen buffer code
         * 
         */   
            void alternateScreenBuffer();

        /**
         * @brief ansii remove mouse cursor code
         * 
         */
            void removeMouseCursor();

        /**
         * @brief ansii enable mouse move code
         * 
         */
            void enableMouseMove();

        /**
         * @brief ansii disable mouse move code
         * 
         */
            void disableMouseMove();

        /**
         * @brief ansii enable mouse click code
         * 
         */
            void enableMouseClick();

        /**
         * @brief ansii disable mouse click code
         * 
         */
            void disableMouseClick();

            virtual void draw() = 0;

        /**
         * @brief get the content of the window
         * 
         * @return std::string the content of the window
         */
            [[nodiscard]] std::string getContent() const { return _content; }


        std::string _content; /* !< the content of the window */

    };
}

#endif //TDL_ANSII_CODE_HPP