// Created by Modar Nasser on 01/09/2020.

#include "Game.hpp"
#include "states/LevelState.hpp"
#include "MapCollisions.hpp"

LevelState::LevelState() = default;

LevelState::LevelState(const std::string& map_name) {
    m_map_name = map_name;
    game->scene->clearAllLayers();
    game->ui_scene->clearAllLayers();

    m_map = std::make_shared<ns::tm::TiledMap>();
    m_map->loadFromFile("assets/maps/"+map_name);

    game->player->setPosition(m_map->getProperty<float>("start_x"), m_map->getProperty<float>("start_y"));
    game->player->inputs()->setCaptureInput(true);

    MapCollisions::clear();
    for (auto& rect : m_map->getObjectLayer("collisions")->getRectangles()) {
        MapCollisions::add(ns::FloatRect(rect.getShape().getGlobalBounds()));
    }

    game->scene->getLayer("ground")->add(m_map->getTileLayer("ground"));
    game->scene->getLayer("ground")->add(m_map->getTileLayer("ground2"));
    game->scene->getLayer("front")->add(m_map->getTileLayer("front"));
    game->scene->getLayer("entities")->add(game->player);
    game->scene->getLayer("top")->add(m_map->getTileLayer("top"));
    game->scene->getLayer("top")->add(m_map->getTileLayer("top2"));
    game->scene->getLayer("shapes")->add(m_map->getObjectLayer("collisions"));

    game->camera->follow(game->player.get());
    game->camera->setLimitsRectangle(ns::IntRect(m_map->getTileLayer("ground")->getGlobalBounds()));
}

void LevelState::init() {}

void LevelState::onEvent(const sf::Event& event) {
    m_map->getObjectLayer("collisions")->setVisible(ns::Config::debug);
    if (m_textbox)
        m_textbox->onEvent(event);
}

void LevelState::update() {
    updateMap();
    updateTextbox();

    game->player->update();
}

void LevelState::updateTextbox() {
    if (m_textbox != nullptr && ns::Transition::list.empty()) {
        m_textbox->update();
        if (m_textbox->closed()) {
            game->ui_scene->getDefaultLayer()->remove(m_textbox);
            m_textbox = nullptr;
        }
        game->player->inputs()->setCaptureInput(false);
    }
    else
        game->player->inputs()->setCaptureInput(true);
}

void LevelState::updateMap() {
    m_map->getTileLayer("front")->update();
    m_map->getTileLayer("top")->update();
    if (m_map_name == "egypt_out.tmx")
        m_map->getTileLayer("ground2")->move(0.05, 0);
}