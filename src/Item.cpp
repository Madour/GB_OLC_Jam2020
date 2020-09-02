// Created by Modar Nasser on 01/09/2020.

#include "Item.hpp"

#include "Game.hpp"


Item::Item() = default;

Item::Item(ItemNames item) :
m_name(item)
{}

void Item::use() {
    switch (m_name) {
        default:
            break;
    }
}
