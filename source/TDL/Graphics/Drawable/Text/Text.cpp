
#include "TDL/Text/Text.hpp"
#include "TDL/Matrix/PixelMatrix.hpp"
#include "TDL/Pixel/Pixel.hpp"

namespace tdl {
    Text::Text()
    {
    }

    Text::Text(Font &font, std::string text)
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

    void Text::draw(Drawable *drawable)
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
                    tdl::Pixel pixel = drawable->getPixel(Vector2u(pen_x + x, pen_y + y)) + tdl::Pixel(grayscale, grayscale, grayscale, grayscale) ;
                    Vector2f point = left.transformPoint(pen_x + x, pen_y + y);
                    drawable->setPixel(Vector2u(point.x(), point.y()), pixel);
                    drawable->getMatrix().registerToUpdate(Vector2u(point.x(), point.y()));
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
                        drawable->setPixel(Vector2u(point.x(), point.y()), Pixel(0, 0, 0, 0));
                        drawable->getMatrix().registerToUpdate(Vector2u(point.x(), point.y()));
                    }
                }
                pen_x += _font.getFace()->glyph->advance.x / 64;
            }
        }
        _oldText = _text;

    }
} // namespace tdl