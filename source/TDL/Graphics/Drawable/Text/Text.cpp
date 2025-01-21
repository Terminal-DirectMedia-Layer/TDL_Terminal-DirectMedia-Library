
#include "TDL/Graphics/Drawable/Text/Text.hpp"

#include <TDL/Graphics/Window/Window.hpp>

#include "TDL/Graphics/Drawable/Pixel/Pixel.hpp"

namespace tdl {
    Text::Text() : ADrawable(Vector2u(0, 0))
    {
    }

    Text::Text(Font &font, std::string text) : ADrawable(Vector2u(0, 0))
    {
        _font = font;
        _text = text;
    }

    Text::~Text()
    {
        
    }

    void Text::setFont(Font &font)
    {
        _font = font;
    }

    Font &Text::getFont()
    {
        return _font;
    }

    void Text::setText(std::string text)
    {
        _text = text;
    }

    std::string Text::getText()
    {
        return _text;
    }

    void Text::draw(Window *drawable)
    {

        int pen_x = 0;
        int pen_y = 0;
        Transform left = getTransform();
        for (char c : _text) {
            if (c == '\n') {
                pen_y += _font.getFace()->size->metrics.height / 64;
                pen_x = 0;
                continue;
            }
            if (FT_Load_Char(_font.getFace(), c, FT_LOAD_RENDER)) {
                std::cout << "Failed to load Glyph" << std::endl;
                continue;
            }

            for (int y = 0; y < _font.getFace()->glyph->bitmap.rows; ++y) {
                for (int x = 0; x < _font.getFace()->glyph->bitmap.width; ++x) {
                    u_int8_t grayscale = _font.getFace()->glyph->bitmap.buffer[y * _font.getFace()->glyph->bitmap.pitch + x];
                    Vector2f point = left.transformPoint(pen_x + x, pen_y + y);
                    std::cerr << point.x() << " " << point.y() << "grayscale: " << grayscale << std::endl;
                    Pixel p = Pixel(grayscale, grayscale, grayscale, grayscale);
                    drawable->setPixel(Vector2u(point.x(), point.y()), p);
                }
            }
            pen_x += _font.getFace()->glyph->advance.x / 64;
        }
        if (_oldText.size() > _text.size())
        {
            for (int i = _text.size(); i < _oldText.size(); i++)
            {
                for (int y = 0; y < _font.getFace()->glyph->bitmap.rows; ++y) {
                    for (int x = 0; x < _font.getFace()->glyph->bitmap.width; ++x) {
                        Vector2f point = left.transformPoint(pen_x + x, pen_y + y);
                        drawable->setPixel(Vector2u(point.x(), point.y()), drawable->getBackground());
                    }
                }
                pen_x += _font.getFace()->glyph->advance.x / 64;
            }
        }
        _oldText = _text;

    }
} // namespace tdl