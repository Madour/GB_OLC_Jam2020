// Created by Modar Nasser on 04/09/2020.

#include "Game.hpp"
#include "states/GameOverState.hpp"
#include "states/TitleScreenState.hpp"

void GameOverState::init() {
    game->scene->clearAllLayers();
    game->ui_scene->getDefaultLayer()->clear();

    for (int i = 0; i < m_game_over_frames.size(); ++i)
        m_game_over_frames[i] = {0, i*49-49, 112, 49};

    m_game_over_sprite = std::make_shared<sf::RectangleShape>(sf::Vector2f(112, 49));
    m_game_over_sprite->setTexture(&ns::Res::getTexture("game_over.png"));
    m_game_over_sprite->setTextureRect({112, 0, 0, 0});
    m_game_over_sprite->setPosition(80-56, 20);

    m_press_start = std::make_shared<ns::BitmapText>("Press START");
    m_press_start->setFont(game->fonts["bold"]);
    m_press_start->setPosition(80-m_press_start->getWidth()/2, 100);
    m_press_start->setColor(sf::Color::Transparent);
    m_to_retry = std::make_shared<ns::BitmapText>("to retry");
    m_to_retry->setFont(game->fonts["bold"]);
    m_to_retry->setPosition(80-m_to_retry->getWidth()/2, 110);
    m_to_retry->setColor(sf::Color::Transparent);

    game->ui_scene->getDefaultLayer()->add(m_game_over_sprite);
    game->ui_scene->getDefaultLayer()->add(m_press_start);
    game->ui_scene->getDefaultLayer()->add(m_to_retry);

}

void GameOverState::onEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed)
        if (event.key.code == ns::Config::Inputs::getButtonKey("start")) {
            auto* tr = new PaletteShiftOutTransition();
            tr->start();
            tr->setOnEndCallback([](){
                (new ns::transition::CircleOpen())->start();
                game->setState<LevelState>(game->last_map_name);
                game->player->restoreHp(10);
            });
        }
}

void GameOverState::update() {
    if (ns::Transition::list.empty()) {
        if (game->getTick()%15 == 0 && m_anim_index < m_game_over_frames.size()-1) {
            m_anim_index ++;
            m_game_over_sprite->setTextureRect(m_game_over_frames[m_anim_index]);
        }
    }

    if (m_anim_index >= 5 && game->getTick() % 50 == 0) {
        if (m_press_start->getColor() == sf::Color::White) {
            m_press_start->setColor(sf::Color::Transparent);
            m_to_retry->setColor(sf::Color::Transparent);
        }
        else {
            m_press_start->setColor(sf::Color::White);
            m_to_retry->setColor(sf::Color::White);
        }
    }

}
