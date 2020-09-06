// Created by Modar Nasser on 30/08/2020.

#include "Game.hpp"
#include "states/TitleScreenState.hpp"
#include "states/MuseumIntroState.hpp"
#include "states/GameOverState.hpp"
#include "states/WarehouseIntroState.hpp"
#include "states/EndGameState.hpp"


void TitleScreenState::init() {

    game->scene->clearAllLayers();
    game->ui_scene->getDefaultLayer()->clear();

    m_titlescreen = std::make_shared<sf::RectangleShape>(sf::Vector2f(160, 144));
    m_titlescreen->setSize(sf::Vector2f(ns::Config::Window::view_width, ns::Config::Window::view_height));
    m_titlescreen->setTexture(&ns::Res::getTexture("titlescreen.png"));
    for (int i = 0; i < 4; i++)
        m_frames[i] = {0, 144*i, 160, 144};
    m_titlescreen->setTextureRect(m_frames[0]);
    game->ui_scene->getDefaultLayer()->add(m_titlescreen);

    m_cursor_left = std::make_shared<sf::CircleShape>(3);
    m_cursor_left->setFillColor(sf::Color::White);
    m_cursor_left->setOrigin(3, 3);

    m_cursor_right = std::make_shared<sf::CircleShape>(3);
    m_cursor_right->setFillColor(sf::Color::White);
    m_cursor_right->setOrigin(3, 3);


    m_buttons.emplace_back(new ns::BitmapText("Play"));
    m_buttons.emplace_back(new ns::BitmapText("Quit"));

    for (unsigned int i = 0; i < m_buttons.size(); ++i) {
        auto& btn = m_buttons[i];
        btn->setFont(game->fonts["default"]);
        btn->setColor(sf::Color::White);
        btn->setPosition(ns::Config::Window::view_width/2.f - btn->getWidth()/2.f, i*12+50);
        game->ui_scene->getDefaultLayer()->add(btn);
    }

    game->ui_scene->getDefaultLayer()->add(m_cursor_left);
    game->ui_scene->getDefaultLayer()->add(m_cursor_right);
}

void TitleScreenState::onEvent(const sf::Event& event) {
    if (ns::Transition::list.empty())
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == Inputs::getButtonKey("A") || event.key.code == Inputs::getButtonKey("start")) {
                if (m_index == 0) {
                    auto* tr = new ns::transition::CircleClose();
                    tr->start();
                    tr->setOnEndCallback([](){
                        (new ns::transition::CircleOpen())->start();
                        game->setState<WarehouseIntroState>();
                    });
                }
                else if (m_index == 1) {
                    auto* tr = new ns::transition::CircleClose();
                    tr->start();
                    tr->setOnEndCallback([](){
                        game->getWindow().close();
                    });
                }
            }
            else if (event.key.code == sf::Keyboard::Up) {
                m_index = std::max(0, m_index-1);
            }
            else if (event.key.code == sf::Keyboard::Down) {
                m_index = std::min((int)m_buttons.size()-1, m_index+1);
            }
        }
}

void TitleScreenState::update() {
    m_cursor_left->setPosition((int)m_buttons[m_index]->getGlobalBounds().left - 6, m_index*12+50 + 4);
    m_cursor_right->setPosition((int)m_buttons[m_index]->getGlobalBounds().right() + 6, m_index*12+50 + 4);

    anim_counter++;
    if (anim_counter == 15) {
        anim_index = (anim_index +1)%m_frames.size();
        m_titlescreen->setTextureRect(m_frames[anim_index]);
        anim_counter = 0;
    }
}
