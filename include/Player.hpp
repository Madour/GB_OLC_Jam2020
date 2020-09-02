// Created by Modar Nasser on 29/08/2020.

#pragma once

#include "../NasNas.h"
#include "MapCollisions.hpp"
#include "GameAccess.hpp"

class Player : public ns::BaseEntity, GameAccess {
public:
    Player();

    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();

    auto getName() -> const std::string&;
    int getHP() const;
    auto getItems() -> std::vector<int>&;

    void damage();
    void update() override;

private:
    std::string m_name = "Adil";
    int m_hp = 10;
    bool visible = true;
    int m_invisibility_timer = 0;
    int m_blink_timer = 0;
    std::vector<int> m_items = {1, 2, 3};
    std::unique_ptr<ns::Spritesheet> m_spritesheet;
    std::vector<int> m_inventory;
};



