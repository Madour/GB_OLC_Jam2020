// Created by Modar Nasser on 29/08/2020.

#include "MapCollisions.hpp"

std::vector<ns::FloatRect> MapCollisions::m_data = {};

void MapCollisions::clear() {
    MapCollisions::m_data.clear();
}

auto MapCollisions::all() -> std::vector<ns::FloatRect>& {
    return m_data;
}

void MapCollisions::add(const ns::FloatRect& rect) {
    m_data.push_back(rect);
}