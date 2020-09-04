// Created by Modar Nasser on 04/09/2020.

#include "Game.hpp"
#include "states/GameOverState.hpp"

void GameOverState::init() {
    game->scene->clearAllLayers();
    game->ui_scene->getDefaultLayer()->clear();

    for (int i = 0; i < 6; ++i)
        m_game_over_frames[i] = {0, i*49, 112, 49};

    m_game_over_sprite = std::make_shared<sf::RectangleShape>(sf::Vector2f(112, 49));
    m_game_over_sprite->setTexture(&ns::Res::getTexture("game_over.png"));
    m_game_over_sprite->setTextureRect(m_game_over_frames[m_anim_index]);

}

void GameOverState::onEvent(const sf::Event& event) {

}

void GameOverState::update() {

}
