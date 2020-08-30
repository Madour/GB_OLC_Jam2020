// Created by Modar Nasser on 30/08/2020.

#include "Game.hpp"
#include "states/LevelState.hpp"


void LevelState::init() {

    game->scene->clearAllLayers();
    game->ui_scene->clearAllLayers();

    m_map = std::make_shared<ns::tm::TiledMap>();
    m_map->loadFromFile("assets/warehouse.tmx");
    m_map->getObjectLayer("collisions")->setVisible(false);

    MapCollisions::clear();
    for (const auto& rect : m_map->getObjectLayer("collisions")->getRectangles())
        MapCollisions::add(ns::FloatRect(rect.getShape().getGlobalBounds()));

    game->scene->getLayer("ground")->add(m_map->getTileLayer("ground"));
    game->scene->getLayer("front")->add(m_map->getTileLayer("front"));
    game->scene->getLayer("front")->add(game->player);
    game->scene->getLayer("top")->add(m_map->getTileLayer("top"));
    game->scene->getLayer("top")->add(m_map->getObjectLayer("collisions"));
    game->scene->getLayer("top")->add(m_map->getObjectLayer("warpzone"));

    m_textbox = std::make_shared<TextBox>("Hello fam !", game->fonts["default"]);
    game->ui_scene->getDefaultLayer()->add(m_textbox);

    game->camera->follow(*game->player);
    game->camera->setLimitsRectangle(ns::IntRect(m_map->getTileLayer("ground")->getGlobalBounds()));

}

void LevelState::onEvent(const sf::Event& event) {
    m_map->getObjectLayer("collisions")->setVisible(ns::Config::debug);
    if (m_textbox)
        m_textbox->onEvent(event);
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::T) {
            auto tr = new PaletteShiftOutTransition();
            tr->start();
            tr->setOnEndCallback([&](){
                auto tr = new PaletteShiftInTransition();
                tr->start();
                tr->setOnEndCallback([&](){
                    game->ui_scene->getDefaultLayer()->remove(m_textbox);
                    m_textbox = std::make_shared<TextBox>("Date : -2000 before JC. \nLocation : Egypt", game->fonts["italic"]);
                    game->ui_scene->getDefaultLayer()->add(m_textbox);
                });
            });
        }
        if (event.key.code == sf::Keyboard::Y) {
            auto tr = new ns::transition::CircleClose();
            tr->start();
            tr->setOnEndCallback([](){
                auto tr = new ns::transition::CircleOpen();
                tr->start();
            });
        }
    }
}

void LevelState::update() {

    game->player->inputs()->setCaptureInput(true);
    if (m_textbox != nullptr && ns::Transition::list.empty()) {
        m_textbox->update();
        if (m_textbox->closed()) {
            game->ui_scene->getDefaultLayer()->remove(m_textbox);
            m_textbox = nullptr;
        }
        game->player->inputs()->setCaptureInput(false);
    }

    game->player->update();
}
