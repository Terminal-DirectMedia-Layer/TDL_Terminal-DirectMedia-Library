

#ifndef TDL_FONT_HPP
    #define TDL_FONT_HPP

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
namespace tdl {
    /**
     * @class Font
     * @brief Font class
     * 
     */
    class Font {
    public:
    /**
     * @brief Construct a new Font object
     * 
     */
        Font();

    /**
     * @brief Destroy the Font object
     * 
     */
        ~Font();

    /**
     * @brief load a font from a file
     * 
     * @param filename the filename of the font
     */
        void loadFromFile(const std::string &filename);

    /**
     * @brief Set the Size object
     * 
     * @param size the size to set
     */
        void setSize(unsigned int size);

        FT_Face getFace(); /* !< get the face of the font */

    private:
        FT_Library _library; /* !< the library of the font */
        FT_Face _face; /* !< the face of the font */
        unsigned int _size; /* !< the size of the font */
    };
}

#endif //TDL_FONT_HPP
