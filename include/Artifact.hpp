// Created by Modar Nasser on 01/09/2020.

#pragma once

#include "../NasNas.h"
#include "GameAccess.hpp"

enum class ArtifactType {
    None,
    Scarab,
    Skull,
    Vase,
    Ball
};

class Artifact : public ns::Drawable, GameAccess {
public:
    explicit Artifact(ArtifactType type);

    auto getSprite() -> sf::Sprite&;

    void setPosition(float x, float y);
    auto getPosition() -> sf::Vector2f override;
    auto getGlobalBounds() -> ns::FloatRect override;

    void update();

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Sprite m_sprite;
    ArtifactType m_type;
    std::array<ns::IntRect, 2> m_frames;
    int m_frame_index = 0;
    sf::Clock m_clk;
};



