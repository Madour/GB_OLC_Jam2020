// Created by Modar Nasser on 29/08/2020.

#pragma once

#include "../NasNas.h"

#include "Palette.hpp"
#include "Player.hpp"
#include "PaletteShiftTransition.hpp"
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

    template <typename OldState, typename NewState>
    void setState();

    ns::Scene* scene;
    ns::Scene* ui_scene;

    ns::Camera* camera;
    ns::Camera* ui_camera;

    std::map<std::string, std::shared_ptr<ns::BitmapFont>> fonts;

    std::shared_ptr<Player> player;

private:
    void initBitmapFonts();

    int m_ticks = 0;

    GameState* m_state;

    int m_palette_index = 0;
    std::shared_ptr<sf::Shader> m_palette_shader;

};

template <typename OldState, typename NewState>
void Game::setState() {
    m_ticks = 0;
    delete(dynamic_cast<OldState*>(m_state));
    m_state = new NewState();
    m_state->init();
}


