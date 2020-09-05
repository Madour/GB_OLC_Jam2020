// Created by Modar Nasser on 30/08/2020.

#pragma once

#include "LevelState.hpp"
#include "entities/Hackerman.hpp"
#include <list>

class WarehouseIntroState : public LevelState {
    using LevelState::LevelState;
    void init() override;
    void onEvent(const sf::Event& event) override;
    void update() override;

    ns::FloatRect m_warp_zone;
    sf::Clock m_clk;

    std::shared_ptr<Hackerman> m_hackerman;
    std::unique_ptr<ns::Spritesheet> m_machine_active_spritesheet;
    ns::AnimPlayer m_machine_active_anim_player;
    std::shared_ptr<sf::RectangleShape> m_machine_active_sprite;
    std::shared_ptr<sf::RectangleShape> m_splash_hackerman;
    std::shared_ptr<sf::RectangleShape> m_title_hackerman;
    std::shared_ptr<sf::RectangleShape> m_black_screen;

    static std::list<std::string> travels;
};



