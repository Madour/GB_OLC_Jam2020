// Created by Modar Nasser on 01/09/2020.

#include "Item.hpp"

#include "Game.hpp"
#include "transitions/WaveTransition.hpp"
#include "states/WarehouseLevelState.hpp"


Item::Item(ItemNames item) :
m_name(item) {
    m_sprite.setTexture(ns::Res::getTexture("gui.png"));
    switch (m_name) {
        case None:
            m_sprite.setTextureRect({80, 24, 8, 8});
            break;
        case Vulnerary:
            m_sprite.setTextureRect({56, 24, 8, 8});
            break;
        case InvisibilityPill:
            m_sprite.setTextureRect({64, 24, 8, 8});
            break;
        case PresentButton:
            m_sprite.setTextureRect({72, 24, 8, 8});
            break;

    }
}

void Item::use() {
    switch (m_name) {
        case None:
            break;
        case Vulnerary:
            game->player->restore(5);
            break;
        case InvisibilityPill :
            game->player->invisibility();
            break;
        case PresentButton:
            if (game->camera->getShader() == nullptr)
                game->camera->toggleShader();
            auto* tr = new WaveOutTransition();
            tr->start();
            tr->setOnEndCallback([&](){
                game->hud->close();
                game->setState<WarehouseLevelState>();
                auto* tr = new WaveInTransition();
                tr->start();
                tr->setOnEndCallback([](){
                    game->camera->getShader()->setUniform("time", 0.f);
                    game->camera->toggleShader();
                });
            });
            break;
    }
}

void Item::setPosition(float x, float y) {
    m_sprite.setPosition(x, y);
}

auto Item::getPosition() -> sf::Vector2f {
    return m_sprite.getPosition();
}

auto Item::getGlobalBounds() -> ns::FloatRect {
    return ns::FloatRect(m_sprite.getGlobalBounds());
}

void Item::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_sprite, states);
}
