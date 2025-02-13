#include <iostream>
#include <filesystem>
#include "files.hpp"
#include <vector>
#include "TDL/Graphics/Display/Display.hpp"

int main() {
    std::string path = ".";
    std::vector<File> files;

    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        files.push_back(File(entry.path().string()));
    }

    tdl::Window win("file system", tdl::Vector2u(800,600));
    win.setPosition(tdl::Vector2u(100,100));
    tdl::Display::getInstance().addWindow(&win);
    int j = 0;
    int x, y = 10;
    for (int i = 0; i < files.size(); i++) {
        if (files[i]._sprite.getSize().x() + 10 + i * files[i]._sprite.getSize().x() > win.getSize().x()) {
            j++;
            x = 15;
        } else {
            x = 15 + i * files[i]._sprite.getSize().x();
        }
        files[i]._sprite.setPosition(x, y + j * files[i]._sprite.getSize().y());
        //files[i]._text.setPosition(x, y + j * files[i]._sprite.getSize().y() + files[i]._sprite.getSize().y());
        win.addDrawable(&files[i]._sprite);
    }

    tdl::Display::getInstance()._interruptManager.start();

    while (tdl::Display::getInstance().isOpen()) {
        tdl::Display::getInstance().clear();
        tdl::Display::getInstance().pollEvent();
        tdl::Display::getInstance().draw();
        tdl::Display::getInstance().fps.frameRendered();
    }

    tdl::Display::getInstance()._interruptManager.stop();
    return 0;
}