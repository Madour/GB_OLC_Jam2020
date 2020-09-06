// Created by Modar Nasser on 30/08/2020.

#include "Game.hpp"
#include "states/MuseumIntroState.hpp"
#include "states/WarehouseIntroState.hpp"

void MuseumIntroState::init() {

    game->scene->clearAllLayers();
    game->ui_scene->getDefaultLayer()->clear();

    game->player->setPosition(184, 66);
    m_hackerman = std::make_shared<Hackerman>();
    m_hackerman->graphics<ns::ecs::SpriteComponent>(0)->setAnimState("sit");
    m_hackerman->setPosition(58, 284);

    m_map = std::make_shared<ns::tm::TiledMap>();
    m_map->loadFromFile("assets/maps/museum_intro.tmx");

    game->scene->getLayer("back")->add(m_map->getTileLayer("back"));
    game->scene->getLayer("back")->add(m_map->getTileLayer("back2"));
    game->scene->getLayer("front")->add(m_map->getTileLayer("front"));
    game->scene->getLayer("entities")->add(game->player);
    game->scene->getLayer("entities")->add(m_hackerman);

    game->scene->getLayer("top")->add(m_map->getTileLayer("top"));
    game->scene->getLayer("top")->add(m_map->getTileLayer("top2"));
    game->scene->getLayer("shapes")->add(m_map->getObjectLayer("collisions"));

    game->camera->follow(game->player.get());
    game->camera->setLimitsRectangle(ns::IntRect(m_map->getTileLayer("back")->getGlobalBounds()));

    LevelState::init();

}

void MuseumIntroState::onEvent(const sf::Event& event) {
    m_map->getObjectLayer("collisions")->setVisible(ns::Config::debug);
    if (m_textbox)
        m_textbox->onEvent(event);
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::T) {
            auto* tr = new PaletteShiftOutTransition();
            tr->start();
            tr->setOnEndCallback([&](){
                auto* tr = new PaletteShiftInTransition();
                tr->start();
                game->setState<WarehouseIntroState>();

            });
        }
        if (event.key.code == sf::Keyboard::Y) {
            auto* tr = new ns::transition::CircleClose();
            tr->start();
            tr->setOnEndCallback([](){
                auto* tr = new ns::transition::CircleOpen();
                tr->start();
            });
        }
    }
}

void MuseumIntroState::update() {
    updateMap();
    updateTextbox();

    if (game->player->getX() <= 80 && game->player->getY() < 200) {
        game->camera->follow(m_hackerman.get());
    }

    if (game->player->getX() <= 80 && game->player->getY() > 235) {
        if (m_hackerman->graphics<ns::ecs::SpriteComponent>(0)->getAnimState() == "sit") {
            m_hackerman->setDestination(60, 260);
            m_hackerman->graphics<ns::ecs::SpriteComponent>(0)->setAnimState("walk_up");
        }
    }

    if (m_textbox == nullptr && game->player->getX() <= 66 && game->player->getY() <= 110) {
        if (ns::Transition::list.empty()) {
            auto* tr = new PaletteShiftOutTransition();
            tr->start();
            tr->setOnEndCallback([](){
                (new PaletteShiftInTransition())->start();
                game->setState<WarehouseIntroState>();
            });
        }
    }

    if (ns::FloatRect(16, 64, 16, 16).contains(game->player->collider()->getCollision().getShape().getGlobalBounds())) {
        game->player->inputs()->setCaptureInput(false);
        if (ns::Transition::list.empty()) {
            auto* tr = new PaletteShiftOutTransition();
            tr->start();
            tr->setOnEndCallback([](){
                game->setState<WarehouseIntroState>();
                auto* tr = new PaletteShiftInTransition();
                tr->start();
            });
        }
    }

    game->player->update();
    m_hackerman->update();
}
