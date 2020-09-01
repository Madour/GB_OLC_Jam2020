// Created by Modar Nasser on 29/08/2020.

#include "transitions/PaletteShiftTransition.hpp"

#include <utility>


PaletteShiftInTransition::PaletteShiftInTransition() :
m_uniform_name("threshold"),
m_shader(app->getShader()),
m_treshold(1.f)
{}

void PaletteShiftInTransition::onUpdate() {
    m_treshold -= 0.025f;
    if (m_treshold <= 0.0) {
        end();
        m_treshold = 0.;
    }
    if (m_shader)
        m_shader->setUniform(m_uniform_name, m_treshold);
}


PaletteShiftOutTransition::PaletteShiftOutTransition() :
m_uniform_name("threshold"),
m_shader(app->getShader()),
m_treshold(0.)
{}

void PaletteShiftOutTransition::onUpdate() {
    m_treshold += 0.025f;
    if (m_treshold >= 1.5)
        end();
    if (m_shader)
        m_shader->setUniform(m_uniform_name, m_treshold);
}
