// Created by Modar Nasser on 29/08/2020.

#pragma once

#include "../NasNas.h"

class Player : public ns::BaseEntity {
public:
    Player();

    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();

    void update() override;

private:
    std::unique_ptr<ns::Spritesheet> m_spritesheet;
    std::vector<int> m_inventory;
};



