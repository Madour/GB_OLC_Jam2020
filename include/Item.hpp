// Created by Modar Nasser on 01/09/2020.

#pragma once

#include "GameAccess.hpp"
#include "../NasNas.h"

class Game;

enum ItemType {
    None,
    Vulnerary,
    InvisibilityPill,
    PresentButton
};

class Item : public ns::Drawable, GameAccess{
public:
    explicit Item(ItemType item);

    void use();
    auto getType() const -> ItemType;

    void setPosition(float x, float y);
    auto getPosition() -> sf::Vector2f override;
    auto getGlobalBounds() -> ns::FloatRect override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Sprite m_sprite;
    ItemType m_type;
    int m_counter = 0;

};
