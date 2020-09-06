// Created by Modar Nasser on 01/09/2020.

#pragma once

#include "../NasNas.h"
#include <array>

class Spike {
public:

    Spike(float x, float y, int freq=80, int offset=0);

    void update();

    void toggle();

    auto isUp() const -> bool;

    void activate();
    void desactivate();

    auto getVertices() -> std::array<sf::Vertex, 4>;

    auto getGlobalBounds() -> ns::FloatRect;

private:
    int m_counter;
    int m_freq;
    bool m_active = true;
    bool m_up = false;
    std::array<sf::Vertex, 4> m_vertices;
};



