// Created by Modar Nasser on 30/08/2020.

#pragma once

#include "GameState.hpp"
#include "TextBox.hpp"
#include "Spike.hpp"

class LevelState : public GameState {
public:
    explicit LevelState(const std::string& map_name, bool start_pos = true);
    LevelState();

    void init() override;
    void onEvent(const sf::Event& event) override;
    void update() override;

protected:
    void configurePalette();
    void updateTextbox();
    void updateMap();
    std::shared_ptr<ns::tm::TiledMap> m_map;
    std::shared_ptr<TextBox> m_textbox = nullptr;
    std::vector<Spike> m_spikes;

private:
    std::string m_map_name;
    sf::VertexArray m_spikes_vertices;
    sf::RenderTexture m_spikes_texture;
    std::shared_ptr<sf::RectangleShape> m_spikes_sprite;
};



