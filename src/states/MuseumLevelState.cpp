// Created by Modar Nasser on 30/08/2020.

#include "Game.hpp"
#include "states/MuseumLevelState.hpp"
#include "states/WarehouseLevelState.hpp"


void MuseumLevelState::init() {

    game->scene->clearAllLayers();
    game->ui_scene->getDefaultLayer()->clear();

    game->player->setPosition(170, 170);

    m_map = std::make_shared<ns::tm::TiledMap>();
    m_map->loadFromFile("assets/maps/museum.tmx");

    game->scene->getLayer("back")->add(m_map->getTileLayer("back"));
    game->scene->getLayer("back")->add(m_map->getTileLayer("back2"));
    game->scene->getLayer("front")->add(m_map->getTileLayer("front"));
    game->scene->getLayer("entities")->add(game->player);

    game->scene->getLayer("top")->add(m_map->getTileLayer("top"));
    game->scene->getLayer("top")->add(m_map->getTileLayer("top2"));
    game->scene->getLayer("shapes")->add(m_map->getObjectLayer("collisions"));
    game->scene->getLayer("shapes")->add(m_map->getObjectLayer("warpzone"));

    m_textbox = std::make_shared<TextBox>("... \n\n\nMy museum is empty today. \nOpened for two monthes, and only 3 visitors... "
                                          "I really have to do something. \nI need to find a way to get more "
                                          "valuable artifacts to show; otherwise my museum will not survive the next month. \n\n\n"
                                          "I am ready to do anything! \nReally, anything !!", game->fonts["default"]);
    game->ui_scene->getDefaultLayer()->add(m_textbox);

    game->camera->follow(game->player.get());
    game->camera->setLimitsRectangle(ns::IntRect(m_map->getTileLayer("back")->getGlobalBounds()));

    LevelState::init();

}

void MuseumLevelState::onEvent(const sf::Event& event) {
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
                game->setState<WarehouseLevelState>();

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

void MuseumLevelState::update() {
    updateMap();
    updateTextbox();

    if (ns::FloatRect(16, 64, 16, 16).contains(game->player->collider()->getCollision().getShape().getGlobalBounds())) {
        game->player->inputs()->setCaptureInput(false);
        if (ns::Transition::list.empty()) {
            auto* tr = new PaletteShiftOutTransition();
            tr->start();
            tr->setOnEndCallback([](){
                game->setState<WarehouseLevelState>();
                auto* tr = new PaletteShiftInTransition();
                tr->start();
            });
        }
    }

    game->player->update();
}
