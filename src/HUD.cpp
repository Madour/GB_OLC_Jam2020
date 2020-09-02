// Created by Modar Nasser on 01/09/2020.

#include "HUD.hpp"
#include "Palette.hpp"
#include "Game.hpp"

HUD::HUD() :
m_timer("00:00"),
m_timer_shadow("00:00") {

    m_timer.setFont(game->fonts.at("bold"));
    m_timer.setColor(Palette::Base[0]);
    m_timer.setPosition(80.f - m_timer.getSize().x/2.f, 2 - 40);
    m_timer_shadow.setFont(game->fonts.at("bold"));
    m_timer_shadow.setColor(Palette::Base[3]);
    m_timer_shadow.setPosition(81.f - m_timer.getSize().x/2.f, 2 - 41);

    m_panel_left.setTexture(ns::Res::getTexture("gui.png"));
    m_panel_left.setTextureRect({0, 0, 55, 20});
    m_panel_left.setPosition(0-2*30, 0);

    m_panel_right.setTexture(ns::Res::getTexture("gui.png"));
    m_panel_right.setTextureRect({55, 0, 55, 20});
    m_panel_right.setPosition(160-55 + 2*30, 0);

    m_hp_bar.setTexture(ns::Res::getTexture("gui.png"));
    m_hp_bar.setTextureRect({0, 20, 31, 7});
    m_hp_bar.setPosition(14 - 2*30, 4);

    m_cursor.setTexture(ns::Res::getTexture("gui.png"));
    m_cursor.setTextureRect({32, 20, 18, 14});
    m_cursor.setPosition(160-19 + 2*30, 1);

    m_clk.restart();

}

void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_panel_left, states);
    target.draw(m_panel_right, states);
    target.draw(m_hp_bar, states);
    target.draw(m_cursor, states);
    target.draw(m_timer, states);
    target.draw(m_timer_shadow, states);
}

void HUD::open() {
    m_opened = true;
}

void HUD::close() {
    m_opened = false;
}

void HUD::setTimerMaxTime(int seconds) {
    m_max_time = seconds;
    m_clk.restart();
}


void HUD::nextItem() {
    m_item_index = (m_item_index + 1)%game->player->getItems().size();
}

void HUD::update() {

    if (m_opened) {
        int current_time = m_max_time - (int)m_clk.getElapsedTime().asSeconds();
        auto minutes = std::max(0, current_time/60);
        auto seconds = std::max(0, current_time%60);
        auto minutes_str = std::to_string(minutes), seconds_str = std::to_string(seconds);
        if (minutes < 10)
            minutes_str = "0"+minutes_str;
        if (seconds < 10)
            seconds_str = "0"+seconds_str;
        m_timer.setString(minutes_str+":"+seconds_str);
        m_timer_shadow.setString(minutes_str+":"+seconds_str);

        m_hp_bar.setTextureRect({0, 20, 3*game->player->getHP(), 7});

        auto cursor_dx = (float)(160-19 - m_item_index*16) - m_cursor.getPosition().x;
        m_cursor.move(cursor_dx/5.f, 0);

        if (m_panel_left.getPosition().x < 0) {
            m_panel_left.move(2, 0);
            m_hp_bar.move(2, 0);
            m_panel_right.move(-2, 0);
            m_cursor.move(-2, 0);
        }
        if (m_timer.getPosition().y < 2) {
            m_timer.move(0, 1);
            m_timer_shadow.move(0, 1);
        }
    }
    else {
        if (m_panel_left.getPosition().x > -2*30) {
            m_panel_left.move(-2, 0);
            m_hp_bar.move(-2, 0);
            m_panel_right.move(2, 0);
            m_cursor.move(2, 0);
        }
        if (m_timer.getPosition().y > -40 && m_panel_left.getPosition().x < -10) {
            m_timer.move(0, -1);
            m_timer_shadow.move(0, -1);
        }
    }

}

auto HUD::getPosition() -> sf::Vector2f {
    return sf::Vector2f(1, 1);
}

auto HUD::getGlobalBounds() -> ns::FloatRect {
    return ns::FloatRect(0, 0, 160, 20);
}
