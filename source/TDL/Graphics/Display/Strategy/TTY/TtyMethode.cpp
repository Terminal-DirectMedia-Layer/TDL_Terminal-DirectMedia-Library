
#include "TDL/Graphics/Display/Strategy/TTY/TtyMethode.hpp"

#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <cstddef>
#include <sys/kd.h>
#include <linux/kd.h>
#include <cstdlib>

tdl::TtyMethode::TtyMethode(FrameBuffer &buffer)
{
    const char *dev = std::getenv("FRAMEBUFFER") ? std::getenv("FRAMEBUFFER") : NULL;
    const char *tty = std::getenv("TTY") ? std::getenv("TTY") : NULL;
    _fd = open("/dev/fb0", O_RDWR);
    _ttyfd = open("/dev/tty3", O_RDWR);
    ioctl(_ttyfd, KDSETMODE, KD_GRAPHICS);

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
    _vinfo_old = _vinfo;

    _vinfo.sync = 0;
    _vinfo.activate = FB_ACTIVATE_NOW;

    if (ioctl(_fd, FBIOPUT_VSCREENINFO, &_vinfo)) {
        throw std::runtime_error("Error setting variable information.");
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
    std::cerr << "Closing tty" << std::endl;
    memcpy(_fbp, _startScreen.get(), _screensize);
    munmap(_fbp, _screensize);
    ioctl(_fd, FBIOPUT_VSCREENINFO, &_vinfo_old);
    close(_fd);
    ioctl(_ttyfd, KDSETMODE, KD_TEXT);

    close(_ttyfd);
}

void tdl::TtyMethode::draw(FrameBuffer &buffer)
{
    if( memcpy(_fbp, buffer.getRawData(), _screensize) == NULL)
        throw std::runtime_error("Error copying the display screen to the framebuffer");

    if (ioctl(_fd, FBIOPAN_DISPLAY, &_vinfo) != 0){
    // panning failed so you can refresh now using this
    	_vinfo.activate = FB_ACTIVATE_NOW | FB_ACTIVATE_FORCE;
    // or (non-blocking) make the update wait for vblank
    // (not supported by all drivers)
    //var.activate = FB_ACTIVATE_VBL;
    // send the modified var data
    	ioctl(_fd, FBIOPUT_VSCREENINFO, &_vinfo);
    }
}