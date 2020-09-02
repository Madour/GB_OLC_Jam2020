// Created by Modar Nasser on 01/09/2020.

#pragma once

#include "GameAccess.hpp"
#include "../NasNas.h"

class Game;

enum ItemNames {
    None,
    Vulnerary,
    InvisibilityPill,
    PresentButton
};

class Item : public ns::Drawable, GameAccess{
public:
    explicit Item(ItemNames item);

    void use();

    void setPosition(float x, float y);
    auto getPosition() -> sf::Vector2f override;
    auto getGlobalBounds() -> ns::FloatRect override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Sprite m_sprite;
    ItemNames m_name;
    int m_counter = 0;

};
