// Created by Modar Nasser on 01/09/2020.

#pragma once

#include "../NasNas.h"
#include "Player.hpp"

class Game;

class HUD : public ns::Drawable{
public:

    explicit HUD(Game* game);

    void open();
    void close();

    void setTimerMaxTime(int seconds);

    void update();

    void nextItem();

    auto getPosition() -> sf::Vector2f override;
    auto getGlobalBounds() -> ns::FloatRect override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    Game* m_game;
    bool m_opened = false;
    int m_item_index = 0;

    ns::BitmapText m_timer;
    ns::BitmapText m_timer_shadow;
    sf::Clock m_clk;
    int m_max_time = 120;

    sf::Sprite m_panel_left;
    sf::Sprite m_panel_right;
    sf::Sprite m_cursor;
    sf::Sprite m_hp_bar;

};


