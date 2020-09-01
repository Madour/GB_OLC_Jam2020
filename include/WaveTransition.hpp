// Created by Modar Nasser on 31/08/2020.

#pragma once

#include "../NasNas.h"

class WaveInTransition : public ns::Transition {
public:
    WaveInTransition();
    void onUpdate() override;

private:
    sf::Clock m_clk;
    sf::Shader* m_shader;
};

class WaveOutTransition : public ns::Transition {
public:
    WaveOutTransition();
    void onUpdate() override;

private:
    bool finished = false;
    sf::Clock m_clk;
    sf::Shader* m_shader;
};



