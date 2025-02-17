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
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <sys/mman.h>
#include <string.h>


extern int framebuffer_count;

#define TDL_FRAMEBUFFER_SECTION_SIZE 10

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
            _frameSize = Vector2u(1, 1);
           	std::string framebuffer_name = "./framebuffer" + std::to_string(framebuffer_count);
            _name = framebuffer_name;
            _frameBufferFD = open(framebuffer_name.c_str(), O_RDWR | O_CREAT, 0644);
           	if (_frameBufferFD == -1) {
                throw std::runtime_error("Error creating framebuffer file");
            }
            framebuffer_count++;
            if (ftruncate(_frameBufferFD, 1 * sizeof(Pixel)) == -1) {
                throw std::runtime_error("Error resizing framebuffer file");
            }
            _frameBufferMap = (char *)mmap(NULL, 1 * sizeof(Pixel), PROT_READ | PROT_WRITE, MAP_SHARED, _frameBufferFD, 0);
			if (_frameBufferMap == MAP_FAILED) {
                throw std::runtime_error("Error mapping framebuffer file");
            }
            memset(_frameBufferMap, 0, 1 * sizeof(Pixel));
        }

        /**
         * @brief Constructor for the Framebuffer class.
         * @param size The size of the framebuffer.
         * @details The constructor initializes the framebuffer with a given size.
         */
        FrameBuffer(Vector2u size) {
			_frameSize = size;
            std::string framebuffer_name = "./framebuffer" + std::to_string(framebuffer_count);
            _name = framebuffer_name;
            _frameBufferFD = open(framebuffer_name.c_str(), O_RDWR | O_CREAT, 0644);
           	if (_frameBufferFD == -1) {
                throw std::runtime_error("Error creating framebuffer file");
            }
            framebuffer_count++;
            u_int memSize = _frameSize.x() * _frameSize.y() * sizeof(Pixel);
            if (ftruncate(_frameBufferFD, memSize) == -1) {
                throw std::runtime_error("Error resizing framebuffer file");
            }
            _frameBufferMap = (char *)mmap(NULL, memSize, PROT_READ | PROT_WRITE, MAP_SHARED, _frameBufferFD, 0);
			if (_frameBufferMap == MAP_FAILED) {
                throw std::runtime_error("Error mapping framebuffer file");
            }
            memset(_frameBufferMap, 0, memSize);
        }

        FrameBuffer(Vector2u size, Pixel background)
        {
          _frameSize = size;
            std::string framebuffer_name = "./framebuffer" + std::to_string(framebuffer_count);
            _name = framebuffer_name;

            _frameBufferFD = open(framebuffer_name.c_str(), O_RDWR | O_CREAT, 0644);
           	if (_frameBufferFD == -1) {
                throw std::runtime_error("Error creating framebuffer file");
            }
            framebuffer_count++;
            u_int memSize = _frameSize.x() * _frameSize.y() * sizeof(Pixel);

            if (ftruncate(_frameBufferFD, memSize) == -1) {
                throw std::runtime_error("Error resizing framebuffer file");
            }
            _frameBufferMap = (char *)mmap(NULL, memSize, PROT_READ | PROT_WRITE, MAP_SHARED, _frameBufferFD, 0);
			if (_frameBufferMap == MAP_FAILED) {
                throw std::runtime_error("Error mapping framebuffer file");
            }
            memset(_frameBufferMap, 0, memSize);
            std::fill(reinterpret_cast<Pixel*>(_frameBufferMap), reinterpret_cast<Pixel*>(_frameBufferMap) + _frameSize.x() * _frameSize.y(), background);
        }

        /**
         * @brief Destructor for the Framebuffer class.
         */
        ~FrameBuffer() {
           close(_frameBufferFD);
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
            if (x >= _frameSize.x() || y >= _frameSize.y() || x < 0 || y < 0) {
                return;
            }
            memcpy(_frameBufferMap + (y * _frameSize.x() + x) * sizeof(Pixel), &color, sizeof(Pixel));
        }

        inline void setPixel(Vector2f pos, Pixel &color)
        {
            setPixel(pos.x(), pos.y(), color);
        }

        /**
         * @brief sets a pixel at a given position.
         * @param pos The position of the pixel.
         * @param color The color of the pixel.
         */
        inline void setPixel(const Vector2u &pos, Pixel &color)
        {
        	setPixel(pos.x(), pos.y(), color);
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
            static Pixel blackPixel(0, 0, 0, 0);
            if (x >= _frameSize.x() || y >= _frameSize.y() || x < 0 || y < 0) {
                return blackPixel;
            }
            return *reinterpret_cast<Pixel*>(_frameBufferMap + (y * _frameSize.x() + x) * sizeof(Pixel));
        }

        /**
         * @brief gets a pixel at a given position.
         * @param pos The position of the pixel.
         * @return The pixel at the given position.
         */
        inline Pixel &getPixel(const Vector2u &pos)
        {
            return getPixel(pos.x(), pos.y());
        }

        /**
         * @brief gets the size of the framebuffer.
         * @return The size of the framebuffer.
         */
       inline Vector2u getSize()
        {
            return _frameSize;
        }

        /**
         * @brief concatenates a vector of pixels to the current frame.
         * @param pixels The vector of pixels to append.
         */
        void append(std::vector<Pixel> &pixels)
        {
        }

        /**
         * @brief resizes the framebuffer.
         * @param size The new size of the framebuffer.
         */
        void resize(Vector2u &size) {
            lock();
            // Unmap the current framebuffer memory
            if (_frameBufferMap && munmap(_frameBufferMap, _frameSize.x() * _frameSize.y() * sizeof(Pixel)) == -1) {
                throw std::runtime_error("Error unmapping framebuffer file");
            }

            // Close the current file descriptor
            if (close(_frameBufferFD) == -1) {
                throw std::runtime_error("Error closing framebuffer file descriptor");
            }

            // Reopen the file descriptor
            _frameBufferFD = open(_name.c_str(), O_RDWR | O_CREAT, 0644);
            if (_frameBufferFD == -1) {
                throw std::runtime_error("Error reopening framebuffer file");
            }

            // Truncate the file to the new size
            u_int memSize = size.x() * size.y() * sizeof(Pixel);
            if (ftruncate(_frameBufferFD, memSize) == -1) {
                throw std::runtime_error("Error resizing framebuffer file");
            }

            // Remap the file to the new size
            _frameBufferMap = (char *)mmap(NULL, memSize, PROT_READ | PROT_WRITE, MAP_SHARED, _frameBufferFD, 0);
            if (_frameBufferMap == MAP_FAILED) {
                throw std::runtime_error("Error mapping framebuffer file");
            }

            // Initialize the new memory to zero
            std::fill(reinterpret_cast<Pixel*>(_frameBufferMap), reinterpret_cast<Pixel*>(_frameBufferMap) + size.x() * size.y(), Pixel(0, 0, 0, 255));

            // Update the frame size
            _frameSize = size;
            unlock();
        }

        /**
         * @brief clears the framebuffer.
         * @note The previous frame is stored in the _previousFrame variable.
         */
        void clear(Pixel background = Pixel(0, 0, 0, 255))
        {

            std::fill(reinterpret_cast<Pixel*>(_frameBufferMap), reinterpret_cast<Pixel*>(_frameBufferMap) + _frameSize.x() * _frameSize.y(), background);
        }

        void lock()
        {
            _mutex.lock();
        }

        void unlock()
        {
            _mutex.unlock();
        }

        char *getRawData()
        {
        	return _frameBufferMap;
        }

        FrameBuffer &getCurrent() {
            return *this;
        }

      protected:
        std::string _name;
        int _frameBufferFD;
        char* _frameBufferMap;
        Vector2u _frameSize;
        std::mutex _mutex;
        int _sectorCount = TDL_FRAMEBUFFER_SECTION_SIZE;
    };
}
#endif //FRAMEBUFFER_HPP
