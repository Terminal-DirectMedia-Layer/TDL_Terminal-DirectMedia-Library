
#ifndef TDL_TEXT_HPP
    #define TDL_TEXT_HPP

#include "TDL/Text/Font/Font.hpp"
#include "TDL/Drawable/Drawable.hpp"
#include "TDL/Matrix/Transform.hpp"
#include "TDL/Matrix/Transformation.hpp"

namespace tdl {
    /**
     * @class Text
     * @brief Text class
     */
    class Text : public Transformable {
    public:

    /**
     * @brief Construct a new Text object
     * 
     */
        Text();

    /**
     * @brief Construct a new Text object
     * 
     * @param font the font to use
     * @param text the text to display
     */
        Text(Font &font, std::string text);

    /**
     * @brief Destroy the Text object
     * 
     */
        ~Text();

    /**
     * @brief Set the Font object
     * 
     * @param font the font to set
     */
        void setFont(Font &font);

    /**
     * @brief Get the Font object
     * 
     */
        Font &getFont();

    /**
     * @brief Set the Text object
     * 
     * @param text the text to set
     */
        void setText(std::string text);

    /**
     * @brief Get the Text object
     * 
     * @return std::string the text
     */
        std::string getText();

    /**
     * @brief Set the Color object
     * 
     * @param color the color to set
     */
        void setColor(Pixel color) { _color = color; }

    /**
     * @brief Get the Color object
     * 
     * @return std::optional<Pixel> the color of the text
     */
        std::optional<Pixel> getColor() { return _color; }

    /**
     * @brief reset the color of the text
     * 
     */
        void resetColor() { _color = std::nullopt; }

    /**
     * @brief draw the text
     * 
     * @param drawable the drawable to draw on
     */
        void draw(Drawable *drawable);

    private:
        Font _font; /* !< the font of the text */
        std::string _text; /* !< the text to display */
        std::string _oldText; /* !< the old text to display */
        std::optional<Pixel> _color; /* !< the color of the text */
    };
}

#endif //TDL_TEXT_HPP
