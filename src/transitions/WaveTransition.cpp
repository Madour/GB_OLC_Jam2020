// Created by Modar Nasser on 31/08/2020.

#include "transitions/WaveTransition.hpp"
#include "transitions/PaletteShiftTransition.hpp"
#include "Game.hpp"

WaveInTransition::WaveInTransition() :
m_shader(app->allCameras()[0]->getShader())
{
    auto* tr = new PaletteShiftInTransition();
    tr->start();
    //game->playSound("wawa");
}

void WaveInTransition::onUpdate() {
    if (m_shader)
        m_shader->setUniform("time", 3.5f-m_clk.getElapsedTime().asSeconds());

    if (m_clk.getElapsedTime().asSeconds() > 3.5f) {
        end();
    }
}


WaveOutTransition::WaveOutTransition() :
m_shader(app->allCameras()[0]->getShader())
{
    game->playSound("wawa");
}

void WaveOutTransition::onUpdate() {
    if (m_shader)
        m_shader->setUniform("time", m_clk.getElapsedTime().asSeconds());

    if (m_clk.getElapsedTime().asSeconds() > 3.8f && !finished) {
        finished = true;
        auto* tr = new PaletteShiftOutTransition();
        tr->start();
    }
    if (m_clk.getElapsedTime().asSeconds() > 4.5f) {
        end();
    }
}
