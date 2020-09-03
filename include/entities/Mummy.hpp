// Created by Modar Nasser on 03/09/2020.

#pragma once

#include "../../NasNas.h"
#include "entities/Enemy.hpp"

class Mummy : public Enemy {
public:
    Mummy();

    void update() override;

};



