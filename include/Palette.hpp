// Created by Modar Nasser on 29/08/2020.



#pragma once

#include "../NasNas.h"

class Palette {
public:
    static std::array<sf::Color, 4> Base;

    enum Color {
        Black,
        Present,
        Red,
        Sands,
        Stone,
        Forest,

        count
    };
    static auto getColor(Color color) -> const std::array<sf::Glsl::Vec3, 4>&;

private:
    const static std::map<Color, std::array<sf::Glsl::Vec3, 4>> m_data;
    static auto  initData() -> std::map<Color, std::array<sf::Glsl::Vec3, 4>>;
};



