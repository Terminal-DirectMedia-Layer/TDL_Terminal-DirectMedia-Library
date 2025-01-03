
#include "TDL/Graphics/Display/Strategy/TtyMethode.hpp"

#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <cstddef>


tdl::TtyMethode::TtyMethode(FrameBuffer &buffer)
{
    _fd = open("/dev/fb0", O_RDWR);
    if (_fd == -1) {
        throw std::runtime_error("opening /dev/fb0");
        return;
    }
    if (ioctl(_fd, FBIOGET_FSCREENINFO, &_finfo)) {
        throw std::runtime_error("Error reading fixed information.");
        return;
    }

    if (ioctl(_fd, FBIOGET_VSCREENINFO, &_vinfo)) {
        throw std::runtime_error("Error reading variable information.");
        return;
    }
    _screensize = _vinfo.xres * _vinfo.yres * _vinfo.bits_per_pixel / 8;
    _fbp = (char *)mmap(0, _screensize, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0);
    if ((intptr_t)_fbp == -1) {
        throw std::runtime_error("Error mapping framebuffer device to memory");
        return;
    }
    _size = Vector2u(_vinfo.xres, _vinfo.yres);
    buffer.resize(_size);
    _startScreen = std::make_unique<char[]>(_screensize);
    memcpy(_startScreen.get(), _fbp, _screensize);

}

tdl::TtyMethode::~TtyMethode()
{
    memcpy(_fbp, _startScreen.get(), _screensize);
    munmap(_fbp, _screensize);
    close(_fd);
}

void tdl::TtyMethode::draw(FrameBuffer &buffer)
{
    if( memcpy(_fbp, buffer.getRawData(), _screensize) == NULL)
        throw std::runtime_error("Error copying the display screen to the framebuffer");
}