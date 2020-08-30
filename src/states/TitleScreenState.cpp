// Created by Modar Nasser on 30/08/2020.

#include "Game.hpp"
#include "states/TitleScreenState.hpp"
#include "states/LevelState.hpp"


void TitleScreenState::init() {

    game->scene->clearAllLayers();
    game->ui_scene->clearAllLayers();

    auto bg = std::make_shared<sf::RectangleShape>();
    bg->setSize(sf::Vector2f(ns::Config::Window::view_width, ns::Config::Window::view_height));
    game->ui_scene->getDefaultLayer()->add(bg);

    m_cursor_left = std::make_shared<sf::CircleShape>(3);
    m_cursor_left->setFillColor(sf::Color::Black);
    m_cursor_left->setOrigin(3, 3);

    m_cursor_right = std::make_shared<sf::CircleShape>(3);
    m_cursor_right->setFillColor(sf::Color::Black);
    m_cursor_right->setOrigin(3, 3);

    m_buttons.emplace_back(new ns::BitmapText("New Game"));
    m_buttons.emplace_back(new ns::BitmapText("Options"));
    m_buttons.emplace_back(new ns::BitmapText("Credits"));
    m_buttons.emplace_back(new ns::BitmapText("Quit"));

    for (unsigned int i = 0; i < m_buttons.size(); ++i) {
        auto& btn = m_buttons[i];
        btn->setFont(game->fonts["default"]);
        btn->setColor(Palette::Base[0]);
        btn->setPosition(ns::Config::Window::view_width/2.f - btn->getWidth()/2.f, i*12+50);
        game->ui_scene->getDefaultLayer()->add(btn);
    }

    game->ui_scene->getDefaultLayer()->add(m_cursor_left);
    game->ui_scene->getDefaultLayer()->add(m_cursor_right);
}

void TitleScreenState::onEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == Inputs::getButtonKey("A") || event.key.code == Inputs::getButtonKey("start")) {
            if (m_index == 0) {
                auto* tr = new ns::transition::CircleClose();
                tr->start();
                tr->setOnEndCallback([](){
                    auto* tr = new ns::transition::CircleOpen();
                    tr->start();
                    game->setState<TitleScreenState, LevelState>();
                });
            }
        }
        else if (event.key.code == sf::Keyboard::Up) {
            m_index = std::max(0u, m_index-1u);
        }
        else if (event.key.code == sf::Keyboard::Down) {
            m_index = std::min((unsigned int)m_buttons.size(), m_index+1u);
        }
    }
}

void TitleScreenState::update() {
    m_cursor_left->setPosition((int)m_buttons[m_index]->getGlobalBounds().left - 6, m_index*12+50 + 4);
    m_cursor_right->setPosition((int)m_buttons[m_index]->getGlobalBounds().right() + 6, m_index*12+50 + 4);
}
