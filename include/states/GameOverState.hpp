// Created by Modar Nasser on 04/09/2020.

#pragma once

#include "states/LevelState.hpp"

class GameOverState : public LevelState {
public:
    using LevelState::LevelState;

    void init() override;

    void onEvent(const sf::Event& event) override;

    void update() override;

private:
    std::shared_ptr<sf::RectangleShape> m_game_over_sprite;
    std::array<sf::IntRect, 7> m_game_over_frames;
    int m_anim_index=0;

    std::shared_ptr<ns::BitmapText> m_press_start;
    std::shared_ptr<ns::BitmapText> m_to_retry;
};



