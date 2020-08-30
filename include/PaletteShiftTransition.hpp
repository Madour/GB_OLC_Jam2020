// Created by Modar Nasser on 29/08/2020.

#pragma once

#include "../NasNas.h"

class PaletteShiftInTransition : public ns::Transition {
public:
    PaletteShiftInTransition();
    void onUpdate() override;

private:
    std::string m_uniform_name;
    sf::Shader* m_shader;
    float m_treshold;
};


class PaletteShiftOutTransition : public ns::Transition {
public:
    explicit PaletteShiftOutTransition();
    void onUpdate() override;

private:
    std::string m_uniform_name;
    sf::Shader* m_shader;
    float m_treshold;
};
