// Created by Modar Nasser on 05/09/2020.

#pragma once

#include "../../NasNas.h"
#include "GameAccess.hpp"

class Hackerman : public ns::BaseEntity, GameAccess {
public:
    Hackerman();

    void setDestination(float x, float y);

    void update() override;

private:
    std::unique_ptr<ns::Spritesheet> m_spritesheet;
    sf::Vector2f m_destination;

};



