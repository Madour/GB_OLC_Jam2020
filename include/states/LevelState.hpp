// Created by Modar Nasser on 30/08/2020.

#pragma once

#include "GameState.hpp"
#include "TextBox.hpp"

class LevelState : public GameState {
public:
    explicit LevelState(const std::string& map_name);
    LevelState();

    void init() override;
    void onEvent(const sf::Event& event) override;
    void update() override;

protected:
    void updateTextbox();
    void updateMap();
    ns::FloatRect m_warp_zone;
    std::shared_ptr<ns::tm::TiledMap> m_map;
    std::shared_ptr<TextBox> m_textbox = nullptr;

private:
    std::string m_map_name;
};



