
#include <optional>
#include <algorithm>
#include <cmath>

#include "TDL/Graphics/Drawable/Sprite/Sprite.hpp"
#include "TDL/Utils/Math/Vector.hpp"
#include "TDL/Utils/Math/Rect.hpp"
#include "TDL/Graphics/Drawable/Pixel/Pixel.hpp"
#include "TDL/Graphics/Window/Window.hpp"
#include "TDL/Utils/Matrix/Transform.hpp"


/**
 * @brief Construct a new tdl::Sprite::Sprite object
 * 
 * @param texture the texture of the sprite
 * @param pos the position of the sprite
 */
tdl::Sprite::Sprite(tdl::Texture *texture, tdl::Vector2u pos): ADrawable(texture->getSize())
{
    _texture = texture;
    _pos = pos;
    _matrix = texture->getTextureData();
    Vector2u size = texture->getSize();
    _matrix.resize(size);
}

/**
 * @brief Construct a new tdl::Sprite::Sprite object
 * 
 * @param texture the texture of the sprite
 * @param pos the position of the sprite
 * @param rect the rect of the sprite
 */
tdl::Sprite::Sprite(Texture *texture, Vector2u pos, RectU rect): ADrawable(texture->getSize())
{
    _texture = texture;
    _pos = pos;
    _rect = rect;
    _matrix = texture->getTextureData();
    Vector2u size = texture->getSize();
    _matrix.resize(size);
    _matrix = _matrix - rect;

}

/**
 * @brief Destroy the tdl::Sprite::Sprite object
 * @note it will delete the texture of the sprite
 */
tdl::Sprite::~Sprite()
{
    //delete _texture;
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

void tdl::Sprite::draw(Window *d) {
    Transform left = getTransform();
    for (u_int32_t y = 0; y < _matrix.getSize().y(); y++) {
        for (u_int32_t x = 0; x < _matrix.getSize().x(); x++) {
            Pixel pixel = _matrix.getElement(x, y);
            Vector2f point = left.transformPoint(x, y);
            u_int32_t pointX = static_cast<u_int32_t>(point.x());
            u_int32_t pointY = static_cast<u_int32_t>(point.y());
            if (pointX < d->getSize().x() && pointY < d->getSize().y()) {
                d->setPixel(pointX, pointY, pixel);
            }
        }
    }
}


/*
 void tdl::Sprite::draw(tdl::Drawable *drawable) {

    Transform left = _texture->getTransform() * this->getTransform();
    PixelMatrix right = _texture->getOriginalImageData() - _texture->getRect();
    Vector2u rightSize = right.getSize();
    Vector2u drawSize = drawable->getMatrix().getSize();
    u_int32_t drawSizeX = drawSize.x();
    u_int32_t drawSizeY = drawSize.y();
    u_int32_t sizeX = rightSize.x();
    u_int32_t sizeY = rightSize.y();
    auto* pRight = right.getRawPixelData();
    auto* pDraw = drawable->getMatrix().getRawPixelData();

    for (unsigned int y = 0; y < sizeY; y++) {
        auto rowStart = pRight + y * sizeX;
        std::transform(rowStart, rowStart + sizeX, pDraw, [&](Pixel& pixel) {
            unsigned int x = &pixel - rowStart; // Calculate the x coordinate
            Vector2f point = left.transformPoint(x, y);
            u_int32_t pointX = static_cast<u_int32_t>(point.x());
            u_int32_t pointY = static_cast<u_int32_t>(point.y());
            if (pointX >= 0 && pointX < drawSizeX && pointY >= 0 && pointY < drawSizeY) {
                drawable->getMatrix().setPixel(pointX, pointY, pixel);
            }
            return pixel;
        });
    }
*/
     /*
     for (unsigned int y = 0; y < sizeY; y++) {
         for (unsigned int x = 0; x < sizeX; x++) {
             pixels = right.getPixel(x, y);
             point = left.transformPoint(x, y);
             pointX = (u_int32_t)(point.x());
             pointY = (u_int32_t)(point.y());
            if (pointX <= drawSizeX && pointY <= drawSizeY) {
                drawable->getMatrix().setPixel(pointX, pointY, pixels);
            }
         }
     }
 }
*/


bool tdl::Sprite::isIntersect(const Vector2i &point)
{
    /*
    Vector2f pos = getPosition();
    //RectU rect = _rect;
    if (point.x() >= pos.x() && point.x() <= pos.x() + rect.width() && point.y() >= pos.y() && point.y() <= pos.y() + rect.height())
        return true;
     */
    return false;
}