// Created by Modar Nasser on 30/08/2020.

#pragma once

#include "../NasNas.h"
#include "TextBox.hpp"

using Inputs = ns::Config::Inputs;

class Game;

class GameState {
public:
    static Game* game;

    virtual void init() = 0;

    virtual void onEvent(const sf::Event& event) = 0;

    virtual void update() = 0;

    virtual ~GameState();

protected:
    friend Game;
    std::shared_ptr<TextBox> m_textbox = nullptr;

};



