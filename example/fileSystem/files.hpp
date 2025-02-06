
#ifndef FILES_HPP
    #define FILES_HPP


    #include <string>
    #include <filesystem>
    #include <iostream>
    #include "TDL/Graphics/Drawable/Sprite/Sprite.hpp"
    #include "TDL/Graphics/Drawable/Text/Text.hpp"

class File {
public:
    File(std::string path) : _path(path), _texture(std::make_shared<tdl::Texture>("../example/assets/folder.png")) {
        _sprite.setTexture(_texture.get());
        _sprite.setPosition(0, 0);
        tdl::RectU rect(0, 0, _texture->getSize().x(), _texture->getSize().y());
        _sprite.setRect(rect);
       // _font.loadFromFile("../example/assets/font.ttf");
        //_font.setSize(20);
        //_text.setFont(_font);
        //_text.setText("Hello World");
        //_text.setPosition(10, 10);
    }

    friend std::ostream& operator<<(std::ostream& os, const File& file);
    tdl::Sprite _sprite;
    //tdl::Text _text;

    private:
        std::string _path;
        std::shared_ptr<tdl::Texture> _texture;
        //tdl::Font _font;


   };

#endif //FILES_HPP

