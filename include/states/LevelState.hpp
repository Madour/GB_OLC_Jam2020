// Created by Modar Nasser on 30/08/2020.

#pragma once

#include "GameState.hpp"

class LevelState : public GameState {

protected:
    ns::FloatRect m_warp_zone;
    std::shared_ptr<ns::tm::TiledMap> m_map;
    std::shared_ptr<TextBox> m_textbox = nullptr;
};



