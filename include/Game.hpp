// Created by Modar Nasser on 29/08/2020.

#pragma once

#include "../NasNas.h"

#include "Palette.hpp"
#include "Player.hpp"
#include "PaletteShiftTransition.hpp"
#include "WaveTransition.hpp"
#include "TextBox.hpp"
#include "MapCollisions.hpp"
#include "GameState.hpp"

class Game : public ns::App {
public:
    Game();
    ~Game() override;

    void onEvent(const sf::Event& event) override;

    void update() override;

    void setPalette(Palette::Color color);
    void setPalette(int color);

    template <typename NewState>
    void setState(const std::string& map_name = "");

    ns::Scene* scene;
    ns::Scene* ui_scene;

    ns::Camera* camera;
    ns::Camera* ui_camera;

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
void Game::setState(const std::string& map_name) {
    m_ticks = 0;
    delete(m_state);
    if (map_name == "")
        m_state = new NewState();
    else
        m_state = new NewState(map_name);
    m_state->init();
}


