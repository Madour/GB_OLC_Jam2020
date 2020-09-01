// Created by Modar Nasser on 30/08/2020.

#pragma once

#include "LevelState.hpp"

class WarehouseLevelState : public LevelState {
    using LevelState::LevelState;
    void init() override;
    void onEvent(const sf::Event& event) override;
    void update() override;

    ns::FloatRect m_warp_zone;
    sf::Clock m_clk;

    std::unique_ptr<ns::Spritesheet> m_machine_active_spritesheet;
    ns::AnimPlayer m_machine_active_anim_player;
    std::shared_ptr<sf::RectangleShape> m_machine_active_sprite;
};



