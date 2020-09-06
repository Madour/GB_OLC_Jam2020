// Created by Modar Nasser on 06/09/2020.

#pragma once

#include "states/LevelState.hpp"
#include "Artifact.hpp"

class EndGameState : public LevelState {
public:
    using LevelState::LevelState;

    void init() override;

    void onEvent(const sf::Event& event) override;

    void update() override;

private:
    int ticks = 0;

    std::shared_ptr<sf::RectangleShape> m_gg_sprite;
    std::array<sf::IntRect, 7> m_gg_frames;
    int m_anim_index=0;

    std::shared_ptr<ns::BitmapText> m_ccl;
    std::array<std::shared_ptr<Artifact>, 4> m_artifacts;
    std::array<std::shared_ptr<sf::RectangleShape>, 4> m_checkboxes;
};

