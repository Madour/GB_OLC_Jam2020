// Created by Modar Nasser on 03/09/2020.

#pragma once

#include "../../NasNas.h"
#include "entities/Enemy.hpp"

class BigMummy : public Enemy {
public:
    BigMummy();

    void update() override;

private:
    int m_cooldown = 0;
    std::unique_ptr<ns::Spritesheet> m_spritesheet;

};



