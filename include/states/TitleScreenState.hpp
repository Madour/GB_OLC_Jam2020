// Created by Modar Nasser on 30/08/2020.

#pragma once

#include "GameState.hpp"
#include "states/LevelState.hpp"

class TitleScreenState : public LevelState {
public:
    using LevelState::LevelState;

    void init() override;

    void onEvent(const sf::Event& event) override;

    void update() override;

private:
    std::vector<std::shared_ptr<ns::BitmapText>> m_buttons;

    std::array<sf::IntRect, 4> m_frames;
    int anim_index = 0;
    int anim_counter = 0;

    int m_index = 0;
    std::shared_ptr<sf::RectangleShape> m_titlescreen;
    std::shared_ptr<sf::CircleShape> m_cursor_left;
    std::shared_ptr<sf::CircleShape> m_cursor_right;

};



