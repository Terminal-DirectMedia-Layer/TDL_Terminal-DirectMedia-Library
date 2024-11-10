/**
* @file Framebuffer.hpp
 * @brief Header file for the Framebuffer class.
 */

#ifndef FRAMEBUFFER_HPP
    #define FRAMEBUFFER_HPP

#include "TDL/Utils/Matrix/Matrix.hpp"
#include "TDL/Graphics/Drawable/Pixel/Pixel.hpp"

namespace tdl
{

    /**
     * @class FrameBuffer
     * @brief A container for a collection of attachments that define the functionality of a framebuffer object.
     *
     * The Framebuffer class is a basic decorator design pattern that comes with a set of pluggable attachments
     * that can be added to the framebuffer object.
     */
    class FrameBuffer
    {
      public:

        /**
         * @brief Constructor for the Framebuffer class.
         */
        FrameBuffer() : _currentFrame(Vector2u(0, 0)), _previousFrame(Vector2u(0, 0)) {}
        FrameBuffer(Vector2u size) : _currentFrame(size), _previousFrame(size) {}
        ~FrameBuffer() {}

        void setPixel(const Vector2u &pos, Pixel &color)
        {
            _currentFrame.setElement(pos, color);
        }

        void setPixel(u_int32_t x, u_int32_t y, Pixel &color)
        {
            _currentFrame.setElement(x, y, color);
        }

        Pixel getPixel(const Vector2u &pos)
        {
            return _currentFrame.getElement(pos);
        }

        Pixel getPixel(u_int32_t x, u_int32_t y)
        {
            return _currentFrame.getElement(x, y);
        }

        void append(std::vector<Pixel> &pixels)
        {
            _currentFrame.append(pixels);
        }

        void resize(Vector2u &size)
        {
            _currentFrame.resize(size);
        }

        void clear()
        {
            _previousFrame = _currentFrame;
            _currentFrame.clear();
        }

        FrameBuffer operator+(std::vector<Pixel> &pixels)
        {
            FrameBuffer newFrameBuffer = FrameBuffer(_currentFrame.getSize());
            newFrameBuffer.append(pixels);
            return newFrameBuffer;
        }

      protected:
          Matrix<Pixel> _currentFrame;
          Matrix<Pixel> _previousFrame;

    };
}
#endif //FRAMEBUFFER_HPP
