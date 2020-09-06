// Created by Modar Nasser on 31/08/2020.

#pragma once

#include "../../NasNas.h"
#include "GameAccess.hpp"

class WaveInTransition : public ns::Transition, GameAccess {
public:
    WaveInTransition();
    void onUpdate() override;

private:
    sf::Clock m_clk;
    sf::Shader* m_shader;
};

class WaveOutTransition : public ns::Transition, GameAccess {
public:
    WaveOutTransition();
    void onUpdate() override;

private:
    bool finished = false;
    sf::Clock m_clk;
    sf::Shader* m_shader;
};



