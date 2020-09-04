// Created by Modar Nasser on 04/09/2020.

#pragma once

#include "GameState.hpp"

class GameOverState : public GameState {
public:
    void init() override;

    void onEvent(const sf::Event& event) override;

    void update() override;

private:
    std::shared_ptr<sf::RectangleShape> m_game_over_sprite;
    std::array<sf::IntRect, 6> m_game_over_frames;
    int m_anim_index=0;
};



