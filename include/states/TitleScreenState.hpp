// Created by Modar Nasser on 30/08/2020.

#pragma once

#include "GameState.hpp"

class TitleScreenState : public GameState {
public:

    void init() override;

    void onEvent(const sf::Event& event) override;

    void update() override;

private:
    std::vector<std::shared_ptr<ns::BitmapText>> m_buttons;

    unsigned int m_index = 0;
    std::shared_ptr<sf::CircleShape> m_cursor_left;
    std::shared_ptr<sf::CircleShape> m_cursor_right;

};



