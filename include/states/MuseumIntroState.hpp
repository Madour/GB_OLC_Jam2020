// Created by Modar Nasser on 30/08/2020.

#pragma once

#include "LevelState.hpp"
#include "entities/Hackerman.hpp"

class MuseumIntroState : public LevelState {
public:
    using LevelState::LevelState;
    void init() override;
    void onEvent(const sf::Event& event) override;
    void update() override;

private:
    std::shared_ptr<Hackerman> m_hackerman;
    int m_story_index;
};



