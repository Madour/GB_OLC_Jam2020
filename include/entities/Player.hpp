// Created by Modar Nasser on 29/08/2020.

#pragma once

#include "../../NasNas.h"
#include "MapCollisions.hpp"
#include "GameAccess.hpp"
#include "Item.hpp"

class Player : public ns::BaseEntity, GameAccess {
public:
    Player();

    void useItem();
    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();

    auto getName() -> const std::string&;
    int getHP() const;
    auto getFaceDirection() ->sf::Vector2f;

    void addItem(ItemType item_type);
    auto getItems() -> std::array<Item, 3>&;

    bool isInvisible() const;
    void setInvisible();
    void restoreHp(int amount);
    void damage();
    void update() override;

private:
    auto getSprite() -> ns::ecs::SpriteComponent*;
    std::string m_name = "Adil";
    int m_hp = 5;
    int m_hp_to_restore = 0;
    bool visible = true;
    int m_invisibility_timer = 0;
    int m_blink_timer = 0;
    std::array<Item, 3> m_items = {Item(PresentButton), Item(None), Item(None)};
    std::unique_ptr<ns::Spritesheet> m_spritesheet;
    std::vector<int> m_inventory;
};



