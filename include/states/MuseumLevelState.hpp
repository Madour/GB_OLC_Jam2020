// Created by Modar Nasser on 30/08/2020.

#pragma once

#include "LevelState.hpp"

class MuseumLevelState : public LevelState {
    using LevelState::LevelState;
    void init() override;
    void onEvent(const sf::Event& event) override;
    void update() override;
};



