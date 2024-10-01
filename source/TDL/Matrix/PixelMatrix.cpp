
#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>
#include <map>
#include <unordered_map>
#include <array>
#include <iostream>
#include <cstring>
#include <cmath>


#include "TDL/Matrix/PixelMatrix.hpp"

namespace tdl {

    std::vector<const char *> PixelCharMap = {
            " ",
            "🬀",
            "🬁",
            "🬂",
            "🬃",
            "🬄",
            "🬅",
            "🬆",
            "🬇",
            "🬈",
            "🬉",
            "🬊",
            "🬋",
            "🬌",
            "🬍",
            "🬎",
            "🬏",
            "🬐",
            "🬑",
            "🬒",
            "🬓",
            "▌",
            "🬔",
            "🬕",
            "🬖",
            "🬗",
            "🬘",
            "🬙",
            "🬚",
            "🬛",
            "🬜",
            "🬝",
            "🬞",
            "🬟",
            "🬠",
            "🬡",
            "🬢",
            "🬣",
            "🬤",
            "🬥",
            "🬦",
            "🬧",
            "🮈",
            "🬨",
            "🬩",
            "🬪",
            "🬫",
            "🬬",
            "🬭",
            "🬮",
            "🬯",
            "🬰",
            "🬱",
            "🬲",
            "🬳",
            "🬴",
            "🬵",
            "🬶",
            "🬷",
            "🬸",
            "🬹",
            "🬺",
            "🬻",
            "█"
    };

/*
    std::map<std::vector<std::pair<int, int>>, const char *> PixelCharMap = {
            {{{0,0}}, "."},
            {{{0,1}}, "."},
            {{{0,0},{0,1}}, ":"},
            {{{1,0}}, "."},
            {{{0,0},{1,0}}, ":"},
            {{{0,1},{1,0}}, ":"},
            {{{0,0},{0,1},{1,0}}, "+"},
            {{{1,1}}, "."},
            {{{0,0},{1,1}}, ":"},
            {{{0,1},{1,1}}, ":"},
            {{{0,0},{0,1},{1,1}}, "+"},
            {{{1,0},{1,1}}, ":"},
            {{{0,0},{1,0},{1,1}}, "+"},
            {{{0,1},{1,0},{1,1}}, "+"},
            {{{0,0},{0,1},{1,0},{1,1}}, "*"},
            {{{2,0}}, "."},
            {{{0,0},{2,0}}, ":"},
            {{{0,1},{2,0}}, ":"},
            {{{0,0},{0,1},{2,0}}, "+"},
            {{{1,0},{2,0}}, ":"},
            {{{0,0},{1,0},{2,0}}, "+"},
            {{{0,1},{1,0},{2,0}}, "+"},
            {{{0,0},{0,1},{1,0},{2,0}}, "*"},
            {{{1,1},{2,0}}, ":"},
            {{{0,0},{1,1},{2,0}}, "+"},
            {{{0,1},{1,1},{2,0}}, "+"},
            {{{0,0},{0,1},{1,1},{2,0}}, "*"},
            {{{1,0},{1,1},{2,0}}, "+"},
            {{{0,0},{1,0},{1,1},{2,0}}, "*"},
            {{{0,1},{1,0},{1,1},{2,0}}, "*"},
            {{{0,0},{0,1},{1,0},{1,1},{2,0}}, "#"},
            {{{2,1}}, "."},
            {{{0,0},{2,1}}, ":"},
            {{{0,1},{2,1}}, ":"},
            {{{0,0},{0,1},{2,1}}, "+"},
            {{{1,0},{2,1}}, ":"},
            {{{0,0},{1,0},{2,1}}, "+"},
            {{{0,1},{1,0},{2,1}}, "+"},
            {{{0,0},{0,1},{1,0},{2,1}}, "*"},
            {{{1,1},{2,1}}, ":"},
            {{{0,0},{1,1},{2,1}}, "+"},
            {{{0,1},{1,1},{2,1}}, "+"},
            {{{0,0},{0,1},{1,1},{2,1}}, "*"},
            {{{1,0},{1,1},{2,1}}, "+"},
            {{{0,0},{1,0},{1,1},{2,1}}, "*"},
            {{{0,1},{1,0},{1,1},{2,1}}, "*"},
            {{{0,0},{0,1},{1,0},{1,1},{2,1}}, "#"},
            {{{2,0},{2,1}}, ":"},
            {{{0,0},{2,0},{2,1}}, "+"},
            {{{0,1},{2,0},{2,1}}, "+"},
            {{{0,0},{0,1},{2,0},{2,1}}, "*"},
            {{{1,0},{2,0},{2,1}}, "+"},
            {{{0,0},{1,0},{2,0},{2,1}}, "*"},
            {{{0,1},{1,0},{2,0},{2,1}}, "*"},
            {{{0,0},{0,1},{1,0},{2,0},{2,1}}, "#"},
            {{{1,1},{2,0},{2,1}}, "+"},
            {{{0,0},{1,1},{2,0},{2,1}}, "*"},
            {{{0,1},{1,1},{2,0},{2,1}}, "*"},
            {{{0,0},{0,1},{1,1},{2,0},{2,1}}, "#"},
            {{{1,0},{1,1},{2,0},{2,1}}, "*"},
            {{{0,0},{1,0},{1,1},{2,0},{2,1}}, "#"},
            {{{0,1},{1,0},{1,1},{2,0},{2,1}}, "#"},
            {{{0,0},{0,1},{1,0},{1,1},{2,0},{2,1}}, "@"}
    };
*/
    PixelMatrix::PixelMatrix() : _size(Vector2u(0, 0)) {
    }

    PixelMatrix::PixelMatrix(Vector2u size) : _size(std::move(size)) {
        _pixelsTab = std::vector<Pixel>(_size.x() * _size.y(), Pixel(0, 0, 0, 255));
        _pixelsTabBlackScreen = std::vector<Pixel>(_size.x() * _size.y(), Pixel(0, 0, 0, 255));
    }

    PixelMatrix::~PixelMatrix() {}

    void PixelMatrix::setPixel(const Vector2u &pos, Pixel &color) {
        if (pos.x() < _size.x() && pos.y() < _size.y()) {
            _pixelsTab[pos.y() * _size.x() + pos.x()] = color;
        }
    }

    void PixelMatrix::setPixel(u_int32_t x, u_int32_t y, Pixel &color) {
        if (x < _size.x() && y < _size.y()) {
            _pixelsTab[y * _size.x() + x] = color;
        }
    }

    void PixelMatrix::setPixelChar(tdl::Vector2u pos, Pixel* pixels)
    {
        setPixelChar(pos.x(), pos.y(), pixels);
    }

    void PixelMatrix::setPixelChar(u_int32_t x, u_int32_t y, Pixel* pixels)
    {
      	u_int32_t sizeX = _size.x();
        if (x + 2 < sizeX && y + 3 < _size.y()) {
            _pixelsTab[y * sizeX + x] = pixels[0];
            _pixelsTab[y * sizeX + x + 1] = pixels[1];
            _pixelsTab[(y + 1) * sizeX + x] = pixels[2];
            _pixelsTab[(y + 1) * sizeX + x + 1] = pixels[3];
            _pixelsTab[(y + 2) * sizeX + x] = pixels[4];
            _pixelsTab[(y + 2) * sizeX + x + 1] = pixels[5];
        }
    }

/**
 * @brief compute the color of the char at the position pos
 *
 * @param pos the position of the char
 * @return CharColor the color of the char
 */
    struct pixelGroup {
        std::vector<std::pair<int, int>> pos;
        Pixel color;
    };

    tdl::CharColor tdl::PixelMatrix::computeCharColor(Vector2u pos, Pixel* pixels, CharColor &charColor)
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

        // ---------------------------------------------------------------
	/*
        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 2; ++x) {
                _pixelGroups[pixels[y * 2 + x]].push_back({y, x});
            }
        }

        size_t maxCount1 = 0, maxCount2 = 0;
        for (const auto& pair : _pixelGroups) {
            if (pair.second.size() > maxCount1) {
                charColor.BackGround = charColor.ForeGround;
                maxCount2 = maxCount1;
                charColor.ForeGround = pair.first;
                maxCount1 = pair.second.size();
            } else if (pair.second.size() > maxCount2) {
                charColor.BackGround = pair.first;
                maxCount2 = pair.second.size();
            }
        }
        charColor.shape = PixelCharMap[_pixelGroups[charColor.ForeGround]];
        return charColor;
	 */
    }

    void PixelMatrix::append(std::vector<Pixel> &pixels) {
        _pixelsTab.insert(_pixelsTab.end(), pixels.begin(), pixels.end());
        _size = Vector2u(pixels.size() > _size.x() ? pixels.size() : _size.x(), _size.y() + 1);}

    void PixelMatrix::resize(Vector2u &size) {
        _size = size;
        _pixelsTab.resize(_size.x() * _size.y(), Pixel(0, 0, 0, 255));
        _pixelsTabBlackScreen.resize(_size.x() * _size.y(), Pixel(0, 0, 0, 255));
    }

    void PixelMatrix::clear() {
        memcpy(_pixelsTab.data(), _pixelsTabBlackScreen.data(), _pixelsTab.size() * sizeof(Pixel));
    }
}
