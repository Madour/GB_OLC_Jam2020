// Created by Modar Nasser on 29/08/2020.



#pragma once

#include "../NasNas.h"

class Palette {
public:
    enum Color {
        Black,
        Green,
        Blue,
        Yellow,
        Kirokaze,

        count
    };
    static auto getColor(Color color) -> const std::array<sf::Glsl::Vec3, 4>&;

private:
    const static std::map<Color, std::array<sf::Glsl::Vec3, 4>> m_data;
    static auto  initData() -> std::map<Color, std::array<sf::Glsl::Vec3, 4>>;
};



