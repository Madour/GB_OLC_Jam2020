// Created by Modar Nasser on 29/08/2020.

#pragma once

#include "../NasNas.h"

#include "Palette.hpp"
#include "entities/Player.hpp"
#include "entities/Enemy.hpp"
#include "transitions/PaletteShiftTransition.hpp"
#include "transitions/WaveTransition.hpp"
#include "TextBox.hpp"
#include "MapCollisions.hpp"
#include "GameState.hpp"
#include "HUD.hpp"

void resolveCollision(ns::BaseEntity* ent, const ns::FloatRect& rect);

class Game : public ns::App {
public:
    Game();
    ~Game() override;

    void onEvent(const sf::Event& event) override;

    void update() override;

    void setPalette(Palette::Color color);
    void setPalette(int color);

    template <typename NewState>
    void setState(const std::string& map_name = "", bool start_pos = true);


    bool isTextboxOpened();

    ns::Scene* scene;
    ns::Scene* ui_scene;

    ns::Camera* camera;
    ns::Camera* ui_camera;

    std::shared_ptr<HUD> hud;

    std::map<std::string, std::shared_ptr<ns::BitmapFont>> fonts;

    std::shared_ptr<Player> player;

    sf::Clock time;

private:
    void initBitmapFonts();

    int m_ticks = 0;

    GameState* m_state;

    int m_palette_index = 0;
    std::shared_ptr<sf::Shader> m_palette_shader;
    std::shared_ptr<sf::Shader> m_wave_shader;

};

template <typename NewState>
void Game::setState(const std::string& map_name, bool start_pos) {
    m_ticks = 0;
    delete(m_state);
    if (map_name.empty())
        m_state = new NewState();
    else
        m_state = new NewState(map_name, start_pos);
    m_state->init();
}


