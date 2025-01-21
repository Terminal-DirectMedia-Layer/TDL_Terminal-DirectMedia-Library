/**
* @file Framebuffer.hpp
 * @brief Header file for the Framebuffer class.
 */

#ifndef FRAMEBUFFER_HPP
    #define FRAMEBUFFER_HPP

#include "TDL/Graphics/Drawable/Pixel/Pixel.hpp"
#include "TDL/Utils/Matrix/Matrix.hpp"

#include "TDL/Utils/Memory/Arena.hpp"

#include <mutex>

#include <Tracy.hpp>

namespace tdl
{

    /**
     * @class FrameBuffer
     * @brief A container for a collection of attachments that define the functionality of a framebuffer object.
     *
     * The Framebuffer class is a basic decorator design pattern that comes with a set of pluggable attachments
     * that can be added to the framebuffer object.
     *
     * By default the frambuffer object is an buffer of pixels that can be drawn to the screen.
     */
    class FrameBuffer
    {
      public:

        /**
         * @brief Constructor for the Framebuffer class.
         */
        FrameBuffer() {
            _currentFrame = new Matrix<Pixel>();
            _previousFrame = new Matrix<Pixel>();
        }

        /**
         * @brief Constructor for the Framebuffer class.
         * @param size The size of the framebuffer.
         * @details The constructor initializes the framebuffer with a given size.
         */
        FrameBuffer(Vector2u size) {
            _currentFrame = new Matrix<Pixel>(size);
            _previousFrame = new Matrix<Pixel>(size);
        }

        FrameBuffer(Vector2u size, Pixel background)
        {
            _currentFrame = new Matrix<Pixel>(size);
            _previousFrame = new Matrix<Pixel>(size);
          _currentFrame->fill(background);
          _previousFrame->fill(background);
        }

        /**
         * @brief Destructor for the Framebuffer class.
         */
        ~FrameBuffer() {
            delete _currentFrame;
            delete _previousFrame;
        }

        /**
         * @brief sets a pixel at a given position.
         * @param pos The position of the pixel.
         * @param color The color of the pixel.
         */
        inline void setPixel(const Vector2u &pos, Pixel &color)
        {
            _currentFrame->setElement(pos, color);
        }


        /**
         * @overload
         * @brief sets a pixel at a given position.
         * @param x The x position of the pixel.
         * @param y The y position of the pixel.
         * @param color The color of the pixel.
         */
        inline void setPixel(u_int32_t x, u_int32_t y, Pixel &color)
        {
            _currentFrame->setElement(x, y, color);
        }

        inline void setPixel(Vector2f pos, Pixel &color)
        {
            _currentFrame->setElement(pos , color);
        }

        /**
         * @brief gets a pixel at a given position.
         * @param pos The position of the pixel.
         * @return The pixel at the given position.
         */
        inline Pixel &getPixel(const Vector2u &pos)
        {
            return _currentFrame->getElement(pos);
        }

        /**
         * @overload
         * @brief gets a pixel at a given position.
         * @param x The x position of the pixel.
         * @param y The y position of the pixel.
         * @return The pixel at the given position.
         */
        inline Pixel &getPixel(u_int32_t x, u_int32_t y)
        {
            return _currentFrame->getElement(x, y);
        }

        /**
         * @brief gets the size of the framebuffer.
         * @return The size of the framebuffer.
         */
       inline Vector2u getSize()
        {
            return _currentFrame->getSize();
        }

        /**
         * @brief concatenates a vector of pixels to the current frame.
         * @param pixels The vector of pixels to append.
         */
        void append(std::vector<Pixel> &pixels)
        {
            _currentFrame->append(pixels);
        }

        /**
         * @brief resizes the framebuffer.
         * @param size The new size of the framebuffer.
         */
        void resize(Vector2u &size)
        {
            _currentFrame->resize(size);
            _currentFrame->fill(Pixel(0, 0, 0, 255));
            _previousFrame->resize(size);
            _previousFrame->fill(Pixel(0, 0, 0, 255));
        }

        /**
         * @brief clears the framebuffer.
         * @note The previous frame is stored in the _previousFrame variable.
         */
        void clear(Pixel background = Pixel(0, 0, 0, 255))
        {
            _previousFrame = _currentFrame;
            _currentFrame->fill(background);
        }

        void lock()
        {
            _mutex.lock();
        }

        void unlock()
        {
            _mutex.unlock();
        }

        Pixel *getRawData()
        {
            return _currentFrame->getRawData();
        }

        FrameBuffer &getCurrent() {
            return *this;
        }

        /**
         * @brief append operator for the framebuffer.
         * @param pixels The vector of pixels to append.
         */
        FrameBuffer *operator+(std::vector<Pixel> &pixels)
        {
            _currentFrame->append(pixels);
            _previousFrame->append(pixels);
            return this;
        }

      protected:
          Matrix<Pixel> *_currentFrame; /**< The current frame of the framebuffer. */
          Matrix<Pixel> *_previousFrame; /**< The previous frame of the framebuffer. */
            std::mutex _mutex;
    };
}
#endif //FRAMEBUFFER_HPP
