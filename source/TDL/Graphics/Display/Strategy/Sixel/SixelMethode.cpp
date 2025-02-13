
#include "TDL/Graphics/Display/Strategy/Sixel/SixelMethode.hpp"
#include "TDL/Utils/AnsiiCode.hpp"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <iostream>
#include <array>
#include <cstdlib>
#include <cmath>
#include <algorithm>
//utiliser une methode qui regroupe les couleur en sous paquet de 255 avec les coordonée associer
// faire plusieur passe pour afficher l'image en entier

namespace tdl
{
    int write_to_stdout(char *data, int size, void *priv) {
        return fwrite(data, 1, size, stdout);
    }

    void setNonCanonicalMode(int fd, struct termios &old_tio) {
        struct termios new_tio;
        tcgetattr(fd, &old_tio);
        new_tio = old_tio;
        new_tio.c_lflag &=(~ICANON & ~ECHO);
        new_tio.c_cc[VMIN] = 1;
        new_tio.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO,TCSANOW, &new_tio);
    }

    SixelMethode::SixelMethode(FrameBuffer &buffer)
    {
    	struct winsize w{};
    	int param;
        _fd = open("/dev/tty", O_RDWR);
        if (_fd == -1)
        	throw std::runtime_error("Can't open tty");
        struct termios old_tio;
        setNonCanonicalMode(_fd, old_tio);
        ioctl(_fd, TIOCGWINSZ, &w);
		if (w.ws_col == 0 || w.ws_row == 0)
   			throw std::runtime_error("Can't get terminal size");
		param = ioctl(_fd, F_GETFL, 0);
		fcntl(_fd, F_SETFL, param | O_NONBLOCK );
        alternateScreenBuffer();
        write(_fd, _content.c_str(), _content.size());
       	Vector2u size = getSixelSize(_fd);
        buffer.resize(size);

        SIXELSTATUS status = sixel_output_new(&_output, write_to_stdout, NULL,NULL);
        if (status != SIXEL_OK) {
            throw std::runtime_error("Erreur d'initialisation de libsixel");
        }
        _dither = sixel_dither_get(SIXEL_BUILTIN_XTERM256);
        if (_dither == NULL) {
            sixel_output_unref(_output);
            throw std::runtime_error("Erreur lors de la création du dither");
        }
    }

    void SixelMethode::updateSize(FrameBuffer &buffer)
    {
        struct winsize w{};
        ioctl(_fd, TIOCGWINSZ, &w);
        if (w.ws_col == 0 || w.ws_row == 0)
            throw std::runtime_error("Can't get terminal size");
        buffer.lock();
        Vector2u size = getSixelSize(_fd);
        buffer.resize(size);
        buffer.unlock();
        SIXELSTATUS status = sixel_output_new(&_output, write_to_stdout, NULL,NULL);
        if (status != SIXEL_OK) {
            throw std::runtime_error("Erreur d'initialisation de libsixel");
        }

        _dither = sixel_dither_get(SIXEL_BUILTIN_XTERM256);
        if (_dither == NULL) {
            sixel_output_unref(_output);
            throw std::runtime_error("Erreur lors de la création du dither");
        }
    }

    void SixelMethode::draw(FrameBuffer &buffer) {
        std::vector<unsigned char> sixel;
        Pixel p;
        for (int y = 0; y < buffer.getSize().y(); y++) {
            for (int x = 0; x < buffer.getSize().x(); x++) {
                p = buffer.getPixel(x, y);
                sixel.push_back(GET_R(p.color));
                sixel.push_back(GET_G(p.color));
                sixel.push_back(GET_B(p.color));
            }
        }

        SIXELSTATUS status = sixel_encode(
            sixel.data(),
            buffer.getSize().x(),
            buffer.getSize().y(),
            3 * buffer.getSize().x(),
            _dither,
            _output);

        if (status != SIXEL_OK) {
            std::cerr << "Erreur lors de l'encodage SIXEL: code " << status << std::endl;
        }
    }
    /*
     	int line = 0;
        int pack_count = 0;
        FrameBuffer buff = atkinsonDithering(buffer);

        moveCursor(Vector2u(0, 0));

		for (u_int32_t y = 0; y < buff.getSize().y() - 6; y += 6) {
            if (line % 6 == 0) {
                startSixel();

            }
        	for (u_int32_t x = 0; x < buff.getSize().x(); x++) {
            	try {
            		Sixel sixel(x, y, buff);
                    transform(sixel);
                    _maskIndex++;

				} catch (std::runtime_error &e) {
					std::cerr << e.what();
				}
			}
			int maskSize = _mask.size();
			int counter = 0;

			for (auto const& [key, val] : _mask) {
    			setSixelColor(key, Pixel(0, 0, 0, 0), true);
    			turnSixelOn(val);
    			if (++counter < maskSize) {
        			jumpSixelColumn();
    			}
            }
            _maskIndex = 0;
            _mask.clear();

    		if (_colorIndex > 256) {
        		std::cerr << "Error: too many colors" << std::endl;
    		}
            if (line % 6 == 5) {
                endSixel();
                _colorMap.clear();
        		_colorIndex = 1;
            } else
            	jumpSixelLine();
            line++;
		}
        if (!_content.empty()) {
        	write(_fd, _content.c_str(), _content.size());
    		_content = "";
    	}
    }

    void SixelMethode::transform(Sixel &sixel)
    {
        std::vector<Pixel> uniqueColor;
        for (int i = 0; i < 6; i++) {
            if (_colorMap.find(sixel._sixel[i].color) == _colorMap.end()) {
                _colorMap[sixel._sixel[i].color] = _colorIndex;
                _colorIndex++;
            }
            if (std::find(uniqueColor.begin(), uniqueColor.end(), sixel._sixel[i]) == uniqueColor.end()) {
                uniqueColor.push_back(sixel._sixel[i]);
            }
        }
        for (Pixel pix : uniqueColor) {
            setSixelColor(_colorMap[pix.color], pix);
            unsigned char mask = 0;
            for (int i = 0; i < 6; i++) {
                if (sixel._sixel[i].color == pix.color) {
                   mask |= (1 << i);
               }
            }
            if (_mask.find(_colorMap[pix.color]) == _mask.end()) {
              for (int i = 0; i <= _maskIndex; i++)
              	_mask[_colorMap[pix.color]] += '?';
            } else {
            	for (auto const& [key, val] : _mask) {
                	if (key != _colorMap[pix.color]) {
                    	_mask[key] += '?';
                	}
            	}
            }
            if (_mask[_colorMap[pix.color]].size() >= _maskIndex) {
                _mask[_colorMap[pix.color]][_maskIndex] = mask + '?';
            } else {
                _mask[_colorMap[pix.color]] += mask + '?';
            }
        }
    }

    void SixelMethode::generatePalette()
    {
    	for (int r = 0; r < 256; r += 32) {
        	for (int g = 0; g < 256; g += 32) {
            	for (int b = 0; b < 256; b += 32) {
                	_colorPalette.push_back({static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), 255});
            	}
        	}
    	}
        _colorPalette.push_back({0, 0, 0, 255});
        _colorPalette.push_back({255, 255, 255, 255});
    }

    Pixel SixelMethode::findClosestColor(Pixel pixel)
    {
        int minDist = 100;
        Pixel closestColor;
        for (Pixel color : _colorPalette) {
			int dist = 	std::abs(static_cast<int>(GET_R(color.color)) - static_cast<int>(GET_R(pixel.color))) +
           				std::abs(static_cast<int>(GET_G(color.color)) - static_cast<int>(GET_G(pixel.color))) +
           				std::abs(static_cast<int>(GET_B(color.color)) - static_cast<int>(GET_B(pixel.color)));
            if (dist < minDist) {
                minDist = dist;
                closestColor = color;
            }
        }
        return closestColor;
    }

	FrameBuffer SixelMethode::atkinsonDithering(FrameBuffer buff)
    {
          const int diffusion[3][3] = {
			{0, 1, 1},
			{1, 1, 1},
			{0, 1, 0},
       	  };
          int width = buff.getSize().x();
          int height = buff.getSize().y();
          const int normalizer = 6;

          for (int y = 0; y < height; y++) {
              for (int x = 0; x < width; x++) {
                  Pixel oldPixel = buff.getPixel(x, y);
                  Pixel newPixel = findClosestColor(oldPixel);
                  buff.setPixel(x, y, newPixel);
                  int errorR = GET_R(oldPixel.color) - GET_R(newPixel.color);
                  int errorG = GET_G(oldPixel.color) - GET_G(newPixel.color);
                  int errorB = GET_B(oldPixel.color) - GET_B(newPixel.color);
                  buff.setPixel(x, y, newPixel);
				  for (int i = 0; i < 3; ++i) {
    				  for (int j = -1; j < 2; ++j) {
						  int x1 = std::clamp(x + j, 0, width - 1);
						  int y1 = std::clamp(y + i, 0, height - 1);
        				  if (x1 < width && y1 < height) {
            				  Pixel pixel = buff.getPixel(x1, y1);
                              int weight = diffusion[i][j + 1];
							  if (diffusion[i][j + 1] == 0) continue;
                              Pixel p(
                                  std::clamp(static_cast<int>GET_R(pixel.color) + (errorR * weight) / normalizer, 0, 255),
          	                      std::clamp(static_cast<int>GET_G(pixel.color) + (errorG * weight) / normalizer, 0, 255),
                                  std::clamp(static_cast<int>GET_B(pixel.color) + (errorB * weight) / normalizer, 0, 255),
                                  255
                              );
           					  buff.setPixel(x1, y1, p);
        				  }
    				  }
                  }
              }
          }
          return buff;
    }
        */

}