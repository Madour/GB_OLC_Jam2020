// Created by Modar Nasser on 29/08/2020.

#pragma once

#include "../NasNas.h"
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
    auto getItems() -> std::array<Item, 3>&;

    void invisibility();
    void restore(int amount);
    void damage();
    void update() override;

private:
    std::string m_name = "Adil";
    int m_hp = 5;
    int m_hp_to_restore = 0;
    bool visible = true;
    int m_invisibility_timer = 0;
    int m_blink_timer = 0;
    std::array<Item, 3> m_items = {Item(PresentButton), Item(InvisibilityPill), Item(Vulnerary)};
    std::unique_ptr<ns::Spritesheet> m_spritesheet;
    std::vector<int> m_inventory;
};



