// Created by Modar Nasser on 29/08/2020.


#include "Palette.hpp"

auto Palette::initData() -> std::map<Color, std::array<sf::Glsl::Vec3, 4>> {
    std::map<Color, std::array<sf::Glsl::Vec3, 4>> m;
    m[Color::Black][0] = {0., 0., 0.};
    m[Color::Black][1] = {0.39, 0.39, 0.39};
    m[Color::Black][2] = {0.7, 0.7, 0.7};
    m[Color::Black][3] = {1., 1., 1.};

    m[Color::Green][0] = {0.2, 0.35, 0.1};
    m[Color::Green][1] = {0.4, 0.7, 0.3};
    m[Color::Green][2] = {0.6, 0.8, 0.6};
    m[Color::Green][3] = {0.9, 0.99, 0.9};

    m[Color::Blue][0] = {0./255, 43./255, 89./255};
    m[Color::Blue][1] = {0./255, 95./255, 140./255};
    m[Color::Blue][2] = {0./255, 185./255, 190./255};
    m[Color::Blue][3] = {170./255, 244./255, 229./255};

    m[Color::Yellow][0] = {0.35, 0.35, 0.1};
    m[Color::Yellow][1] = {0.7, 0.7, 0.3};
    m[Color::Yellow][2] = {0.8, 0.8, 0.6};
    m[Color::Yellow][3] = {0.99, 0.99, 0.9};

    m[Color::Kirokaze][0] = {51./255, 44./255, 80./255};
    m[Color::Kirokaze][1] = {70./255, 135./255, 143./255};
    m[Color::Kirokaze][2] = {148./255, 227./255, 68./255};
    m[Color::Kirokaze][3] = {226./255, 243./255, 228./255};

    return m;
}

const std::map<Palette::Color, std::array<sf::Glsl::Vec3, 4>> Palette::m_data = initData();


auto Palette::getColor(Palette::Color color) -> const std::array<sf::Glsl::Vec3, 4>& {
    return m_data.at(color);
}


