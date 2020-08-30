// Created by Modar Nasser on 29/08/2020.


#include "Palette.hpp"

std::array<sf::Color, 4> Palette::Base = {
        sf::Color::Black,
        sf::Color(100, 100, 100),
        sf::Color(180, 180, 180),
        sf::Color::White
};

auto Palette::initData() -> std::map<Color, std::array<sf::Glsl::Vec3, 4>> {
    std::map<Color, std::array<sf::Glsl::Vec3, 4>> m;
    m[Color::Black][0] = {0, 0, 0};
    m[Color::Black][1] = {100, 100, 100};
    m[Color::Black][2] = {180, 180, 180};
    m[Color::Black][3] = {255, 255, 255};


    m[Color::Present][0] = {0x20, 0x28, 0x3d};
    m[Color::Present][1] = {0x5a, 0x96, 0x7e};
    m[Color::Present][2] = {0xe5, 0xb0, 0x83};
    m[Color::Present][3] = {0xfb, 0xf7, 0xf3};

    m[Color::Red][0] = {0x1b, 0x03, 0x26};
    m[Color::Red][1] = {0x7a, 0x1c, 0x4b};
    m[Color::Red][2] = {0xba, 0x50, 0x44};
    m[Color::Red][3] = {0xef, 0xf9, 0xd6};

    m[Color::Sands][0] = {0x0a, 0x16, 0x4e};
    m[Color::Sands][1] = {0xa2, 0x51, 0x30};
    m[Color::Sands][2] = {0xce, 0xad, 0x6b};
    m[Color::Sands][3] = {0xfa, 0xfd, 0xff};

    m[Color::Stone][0] = {0x08, 0x26, 0x3b};
    m[Color::Stone][1] = {0x30, 0x61, 0x7b};
    m[Color::Stone][2] = {0x72, 0xa8, 0xb8};
    m[Color::Stone][3] = {0xe5, 0xf1, 0xf3};

    m[Color::Forest][0] = {51, 44, 80};
    m[Color::Forest][1] = {70, 135, 143};
    m[Color::Forest][2] = {148, 227, 68};
    m[Color::Forest][3] = {226, 243, 228};

    return m;
}

const std::map<Palette::Color, std::array<sf::Glsl::Vec3, 4>> Palette::m_data = initData();


auto Palette::getColor(Palette::Color color) -> const std::array<sf::Glsl::Vec3, 4>& {
    return m_data.at(color);
}


