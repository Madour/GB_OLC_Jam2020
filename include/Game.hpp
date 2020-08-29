// Created by Modar Nasser on 29/08/2020.

#pragma once

#include "../NasNas.h"
#include "Palette.hpp"
#include "Player.hpp"
#include "ShaderTransition.hpp"
#include "TextBox.hpp"
#include "MapCollisions.hpp"

class Game : public ns::App {
public:
    Game();

    void onEvent(const sf::Event& event) override;

    void update() override;

    void setPalette(Palette::Color color);
    void setPalette(int color);

private:
    void initBitmapFonts();

    sf::Clock m_clock;
    int m_ticks = 0;
    std::map<std::string, std::shared_ptr<ns::BitmapFont>> m_fonts;

    ns::tm::TiledMap m_map;

    std::shared_ptr<TextBox> m_textbox;
    std::shared_ptr<Player> m_player;

    ns::Scene* m_scene;
    ns::Scene* m_ui_scene;

    ns::Camera* m_camera;
    ns::Camera* m_ui_camera;

    int m_palette_index = 0;
    std::shared_ptr<sf::Shader> m_palette_shader;

};
