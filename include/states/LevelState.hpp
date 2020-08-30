// Created by Modar Nasser on 30/08/2020.

#pragma once

#include "GameState.hpp"

class LevelState : public GameState {

    void init() override;

    void onEvent(const sf::Event& event) override;

    void update() override;

private:
    std::shared_ptr<ns::tm::TiledMap> m_map;

    std::shared_ptr<TextBox> m_textbox = nullptr;
};



