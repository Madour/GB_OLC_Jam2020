// Created by Modar Nasser on 01/09/2020.

#include "Game.hpp"
#include "Artifact.hpp"

Artifact::Artifact(ArtifactType type) :
m_type(type) {
    m_sprite.setTexture(ns::Res::getTexture("items.png"));
    switch(m_type) {
        case ArtifactType::Scarab:
            m_frames[0] = {0, 0, 32, 32};
            m_frames[1] = {32, 0, 32, 32};
            break;
        case ArtifactType::Skull:
            m_frames[0] = {64, 32, 32, 32};
            m_frames[1] = {96, 32, 32, 32};
            break;
        case ArtifactType::Vase:
            m_frames[0] = {0, 32, 32, 32};
            m_frames[1] = {32, 32, 32, 32};
            break;
        case ArtifactType::Ball:
            m_frames[0] = {64, 0, 32, 32};
            m_frames[1] = {96, 0, 32, 32};
            break;
        default:
            m_frames[0] = {128, 0, 32, 32};
            m_frames[1] = {128, 0, 32, 32};
            break;
    }
    m_sprite.setTextureRect(m_frames[0]);
    m_sprite.setOrigin(16, 16);
}

auto Artifact::getSprite() -> sf::Sprite& {
    return m_sprite;
}

void Artifact::setPosition(float x, float y) {
    m_sprite.setPosition(x, y);
}

auto Artifact::getPosition() -> sf::Vector2f {
    return m_sprite.getPosition() + sf::Vector2f(0, 16);
}

auto Artifact::getGlobalBounds() -> ns::FloatRect {
    return ns::FloatRect(m_sprite.getGlobalBounds());
}

void Artifact::update() {
    if (m_clk.getElapsedTime().asMilliseconds() > 500) {
        m_frame_index = (++m_frame_index)%2;
        m_sprite.setTextureRect(m_frames[m_frame_index]);
        m_clk.restart();
    }

    if (sf::Keyboard::isKeyPressed(ns::Config::Inputs::getButtonKey("A"))) {
        if (game->player->getGlobalBounds().intersects(m_sprite.getGlobalBounds())) {
            game->level_cleared = true;
            m_sprite.setColor(sf::Color::Transparent);
        }
    }
}

void Artifact::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_sprite, states);
}
