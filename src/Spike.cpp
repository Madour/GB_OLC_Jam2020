// Created by Modar Nasser on 01/09/2020.

#include "Spike.hpp"

Spike::Spike(float x, float y, int freq, int offset) :
m_counter(offset),
m_freq(freq) {
    m_vertices[0].position = {x, y};
    m_vertices[0].texCoords = {80, 64};
    m_vertices[1].position = {x+16, y};
    m_vertices[1].texCoords = {80+16, 64};
    m_vertices[2].position = {x+16, y+16};
    m_vertices[2].texCoords = {80+16, 64+16};
    m_vertices[3].position = {x, y+16};
    m_vertices[3].texCoords = {80, 64+16};
}

void Spike::toggle() { m_up = !m_up; }

auto Spike::isUp() const -> bool { return m_up; }

void Spike::activate() { m_active = true; }
void Spike::desactivate() { m_active = false; }

void Spike::update() {
    if (m_active) {
        if (m_counter % m_freq == 0) {
            toggle();
            m_counter = 0;
        }
        m_counter ++;
    }
    else
        m_counter = 0;

    if (!m_up) {
        for (auto& vert : m_vertices) vert.color = sf::Color::Transparent;
    }
    else {
        if (m_counter < 30)
            m_counter += std::rand()%2;
        for (auto& vert : m_vertices) vert.color = sf::Color::White;
    }
}

auto Spike::getVertices() -> std::array<sf::Vertex, 4> {
    return m_vertices;
}

auto Spike::getGlobalBounds() -> ns::FloatRect {
    return ns::FloatRect(m_vertices[0].position+sf::Vector2f(2, 1), {12, 12});
}
