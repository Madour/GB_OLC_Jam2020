// Created by Modar Nasser on 29/08/2020.

#pragma once

#include "../NasNas.h"

class MapCollisions {
public:
    static auto all() -> std::vector<ns::FloatRect>&;
    static void add(const ns::FloatRect& rect);
    static void clear();

private:
    static std::vector<ns::FloatRect> m_data;

};



