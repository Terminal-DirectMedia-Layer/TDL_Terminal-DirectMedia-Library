
#include "TDL/Graphics/Display/Strategy/Ascii/AsciiMethode.hpp"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

namespace tdl
{
    /**
     * @brief The ASCII pixel char map.
     * each entry of the table correspond to an binary number for 6 pixel conversion
     */
    std::vector<const char *> PixelCharMap = {
        " ", "🬀", "🬁", "🬂", "🬃", "🬄", "🬅", "🬆", "🬇", "🬈", "🬉", "🬊", "🬋", "🬌", "🬍", "🬎", "🬏", "🬐", "🬑",
        "🬒", "🬓", "▌", "🬔", "🬕", "🬖", "🬗", "🬘", "🬙", "🬚", "🬛", "🬜", "🬝", "🬞", "🬟", "🬠", "🬡", "🬢", "🬣",
        "🬤", "🬥", "🬦", "🬧", "🮈", "🬨", "🬩","🬪","🬫","🬬","🬭","🬮","🬯","🬰","🬱","🬲","🬳","🬴","🬵","🬶","🬷",
        "🬸","🬹","🬺","🬻","█"
};

    AsciiMethode::AsciiMethode(FrameBuffer &buffer)
    {
    	struct winsize w{};
    	int param;
        _fd = open("/dev/tty", O_RDWR);
        if (_fd == -1)
        	throw std::runtime_error("Can't open tty");
        ioctl(_fd, TIOCGWINSZ, &w);
		if (w.ws_col == 0 || w.ws_row == 0)
   			throw std::runtime_error("Can't get terminal size");
		param = ioctl(_fd, F_GETFL, 0);
		fcntl(_fd, F_SETFL, param | O_NONBLOCK);
        Vector2u size((w.ws_col + 1) * 2, ((w.ws_row + 1) * 3));
        disableScrolling();
        buffer.resize(size);
    }

    void AsciiMethode::updateSize(FrameBuffer &buffer) {
        struct winsize w{};
        ioctl(_fd, TIOCGWINSZ, &w);
        if (w.ws_col == 0 || w.ws_row == 0)
            throw std::runtime_error("Can't get terminal size");
        buffer.lock();
        Vector2u newSize((w.ws_col + 1) * 2, ((w.ws_row + 1) * 3));
        buffer.resize(newSize);
        buffer.unlock();


    }

    void AsciiMethode::draw(FrameBuffer &buffer)
    {
        CharColor charColor;
        Vector2u pos = Vector2u(0, 0);
        Vector2u oldPos = Vector2u(0, 0);
        Pixel pixels[6] = {Pixel(0, 0, 0, 0)};
        Pixel oldPixels[6] = {Pixel(0, 0, 0, 0)};
        u_int32_t sizeX = buffer.getSize().x();
        u_int32_t sizeY = buffer.getSize().y();
        std::cerr << "sizeX: " << sizeX << " sizeY: " << sizeY << std::endl;
        Pixel oldForeColor = Pixel(0, 0, 0, 0);
        Pixel oldBackColor = Pixel(0, 0, 0, 0);

        for (u_int32_t i = 0; i < sizeY; i += 3) {
            for (u_int32_t j = 0; j < sizeX; j += 2) {
                if (getPixelChar(j, i, pixels, buffer) == false)
                    continue;
                computeCharColor(Vector2u(j, i), pixels, charColor);
                if (!charColor.shape)
                    continue;
                if (pos.x() != oldPos.x() + 2) {
                    moveCursor(pos);
                }
                if (charColor.ForeGround != oldForeColor) {
                    setRGBFrontGround(charColor.ForeGround);
                    oldForeColor = charColor.ForeGround;
                }
                if ((charColor.BackGround != oldBackColor)) {
                    setRGBBackGround(charColor.BackGround);
                    oldBackColor = charColor.BackGround;
                }
                printPixel(charColor.shape);
                oldPos = pos;
                pos.x() += 2;
            }
            pos.x() = 0;
            pos.y() += 2;
        }

        if (!_content.empty()) {
        	write(_fd, _content.c_str(), _content.size());
        _content = "";
    	}
    }

    bool AsciiMethode::getPixelChar(u_int32_t x, u_int32_t y, Pixel *pixel, FrameBuffer &buffer)
    {
        u_int32_t sizeX = buffer.getSize().x();
        u_int32_t sizeY = buffer.getSize().y();
        if (x + 2 < sizeX && y + 3 < sizeY) {
            pixel[0] = buffer.getPixel(x, y);
            pixel[1] = buffer.getPixel(x + 1, y);
            pixel[2] = buffer.getPixel(x, y + 1);
            pixel[3] = buffer.getPixel(x + 1, y + 1);
            pixel[4] = buffer.getPixel(x, y + 2);
            pixel[5] = buffer.getPixel(x + 1, y + 2);
            return true;
        } else {
            return false;
        }
    }

    void AsciiMethode::setPixelChar(u_int32_t x, u_int32_t y, Pixel *pixel, FrameBuffer &buffer)
    {
        u_int32_t sizeX = buffer.getSize().x();
        if (x + 2 < sizeX && y + 3 < buffer.getSize().y()) {
            buffer.setPixel(x, y, pixel[0]);
            buffer.setPixel(x + 1, y, pixel[1]);
            buffer.setPixel(x, y + 1, pixel[2]);
            buffer.setPixel(x + 1, y + 1, pixel[3]);
            buffer.setPixel(x, y + 2, pixel[4]);
            buffer.setPixel(x + 1, y + 2, pixel[5]);
        }
    }

    tdl::CharColor AsciiMethode::computeCharColor(Vector2u pos, Pixel* pixels, CharColor &charColor)
    {
        int primaryMask = 0;
        int secColorI = 0;

        for (int i = 1; i < 6; i++)
            if (pixels[i] != pixels[secColorI]) {
                secColorI = i;
                break;
            }

        for (int i = 0; i < 6; i++) {
            pixels[i] =
                std::max(pixels[0].color, 	      pixels[i].color) - std::min(pixels[0].color, 		   pixels[i].color) <
                std::max(pixels[secColorI].color, pixels[i].color) - std::min(pixels[secColorI].color, pixels[i].color) ?
                pixels[0] : pixels[secColorI];

            primaryMask |= (pixels[i] == pixels[0]) << i;
        }

        charColor.shape = PixelCharMap[primaryMask];
        charColor.ForeGround = pixels[0];
        charColor.BackGround = pixels[secColorI];
        return charColor;
    }
}