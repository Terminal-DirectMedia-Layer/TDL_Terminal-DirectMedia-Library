
#include <optional>
#include <algorithm>
#include <cmath>

#include "TDL/Sprite/Sprite.hpp"
#include "TDL/Math/Vector.hpp"
#include "TDL/Math/Rect.hpp"
#include "TDL/Pixel/Pixel.hpp"
#include "TDL/Window/Window.hpp"
#include "TDL/Drawable/Drawable.hpp"
#include "TDL/Matrix/Transform.hpp"

/**Vector2u pos and a rect
 * 
 * @param texture the texture of the sprite to create
 * @param pos the position of the sprite
 * @param rect the rect of the sprite
 * @return tdl::Sprite* the sprite created
 */
tdl::Sprite* tdl::Sprite::createSprite(tdl::Texture *texture, tdl::Vector2u &pos, tdl::RectU &rect)
{
    return new tdl::Sprite(texture, pos, rect);
}

tdl::Sprite* tdl::Sprite::createSprite(tdl::Texture *texture, tdl::Vector2u pos)
{
    return new tdl::Sprite(texture, pos);
}

/**
 * @brief Construct a new tdl::Sprite::Sprite object
 * 
 * @param texture the texture of the sprite
 * @param pos the position of the sprite
 */
tdl::Sprite::Sprite(tdl::Texture *texture, tdl::Vector2u &pos)
{
    _texture = texture;
    _pos = pos;
    tdl::Vector2u size = texture->getSize();
    _rect = tdl::RectU(0, 0, size.x(), size.y());
}

/**
 * @brief Construct a new tdl::Sprite::Sprite object
 * 
 * @param texture the texture of the sprite
 * @param pos the position of the sprite
 * @param rect the rect of the sprite
 */
tdl::Sprite::Sprite(Texture *texture, Vector2u &pos, RectU &rect)
{
    _texture = texture;
    _pos = pos;
    _rect = rect;
}

/**
 * @brief Destroy the tdl::Sprite::Sprite object
 * @note it will delete the texture of the sprite
 */
tdl::Sprite::~Sprite()
{
    delete _texture;
}

/**
 * @brief check if the pixel is black
 * 
 * @param pixel the pixel to check
 * @return true if the pixel is black
 * @return false if the pixel is not black
 */
bool tdl::Sprite::isBlackPixel(Pixel pixel)
{
    return static_cast<uint8_t>(GET_R(pixel.color)) == 0 && static_cast<uint8_t>(GET_G(pixel.color)) == 0 && static_cast<uint8_t>(GET_B(pixel.color)) == 0;
}

tdl::Pixel tdl::Sprite::lerp(tdl::Pixel a, tdl::Pixel b, double t) {
    return {
        static_cast<uint8_t>((1 - t) * GET_R(a.color) + t * GET_R(b.color)),
        static_cast<uint8_t>((1 - t) * GET_G(a.color) + t * GET_G(b.color)),
        static_cast<uint8_t>((1 - t) * GET_B(a.color) + t * GET_B(b.color)),
        static_cast<uint8_t>((1 - t) * GET_A(a.color) + t * GET_A(b.color))
    };
}

/**
 * @brief draw the sprite on the window
 * This is the main function of the sprite it will draw the sprite according to the variable set in the sprite class
 * this is what is handle :
 * - the position of the sprite
 * - the rect of the sprite
 * - the tint of the sprite
 * - the rotation of the sprite
 * - the texture of the sprite and his repetition
 * 
 * @param window the window to draw the sprite on
 */
 void tdl::Sprite::draw(tdl::Drawable *drawable) {
     Transform left = _texture->getTransform() * this->getTransform();
     PixelMatrix right = _texture->getOriginalImageData() - _texture->getRect();
     Vector2u rightSize = right.getSize();
     Vector2u drawSize = drawable->getMatrix().getSize();
     Vector2u pos = Vector2u(0, 0);
     Pixel pixels;
     Vector2f point;

     for (unsigned int y = 0; y < rightSize.y(); y++) {
         for (unsigned int x = 0; x < rightSize.x(); x++) {
             pixels = right.getPixel(Vector2u(x, y));
             point = left.transformPoint(x, y);
             pos = Vector2u(static_cast<unsigned int>(point.x()), static_cast<unsigned int>(point.y()));
            if (pos <= drawSize) {
                drawable->getMatrix().setPixel(pos, pixels);
            }
         }  
     }
 }

bool tdl::Sprite::isIntersect(const Vector2i &point)
{
    Vector2f pos = getPosition();
    RectU rect = _rect;
    if (point.x() >= pos.x() && point.x() <= pos.x() + rect.width() && point.y() >= pos.y() && point.y() <= pos.y() + rect.height())
        return true;
    return false;
}