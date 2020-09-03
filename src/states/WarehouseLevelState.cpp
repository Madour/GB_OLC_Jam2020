// Created by Modar Nasser on 30/08/2020.

#include "Game.hpp"
#include "states/WarehouseLevelState.hpp"
#include "states/MuseumLevelState.hpp"


void WarehouseLevelState::init() {

    game->scene->clearAllLayers();
    game->ui_scene->getDefaultLayer()->clear();

    m_map = std::make_shared<ns::tm::TiledMap>();
    m_map->loadFromFile("assets/maps/warehouse.tmx");

    game->player->graphics<ns::ecs::SpriteComponent>(0)->setAnimState("idle");
    game->player->setPosition(m_map->getProperty<float>("start_x"), m_map->getProperty<float>("start_y"));

    m_machine_active_spritesheet = std::unique_ptr<ns::Spritesheet>(new ns::Spritesheet(
        "machine_active",
        ns::Res::getTexture("machine_activate.png"),
        {new ns::Anim("idle", {
            ns::AnimFrame({55*0, 0, 55, 25}, 100),
            ns::AnimFrame({55*1, 0, 55, 25}, 100),
            ns::AnimFrame({55*2, 0, 55, 25}, 100),
            ns::AnimFrame({55*3, 0, 55, 25}, 100),
            ns::AnimFrame({55*4, 0, 55, 25}, 100),
            ns::AnimFrame({55*5, 0, 55, 25}, 100),
            ns::AnimFrame({55*6, 0, 55, 25}, 100),
            ns::AnimFrame({55*7, 0, 55, 25}, 100),
            ns::AnimFrame({55*8, 0, 55, 25}, 100),
            ns::AnimFrame({55*9, 0, 55, 25}, 100),
            ns::AnimFrame({55*10, 0, 55, 25}, 100),
            ns::AnimFrame({55*11, 0, 55, 25}, 100),
            ns::AnimFrame({55*12, 0, 55, 25}, 100),
            ns::AnimFrame({55*13, 0, 55, 25}, 100),
            ns::AnimFrame({55*14, 0, 55, 25}, 100),
            ns::AnimFrame({55*15, 0, 55, 25}, 100),
            ns::AnimFrame({55*16, 0, 55, 25}, 100),
            ns::AnimFrame({55*17, 0, 55, 25}, 100),
            ns::AnimFrame({55*18, 0, 55, 25}, 100),
            ns::AnimFrame({55*19, 0, 55, 25}, 100),
        }, false)}
    ));
    m_machine_active_anim_player.play(m_machine_active_spritesheet->getAnim("idle"));
    m_machine_active_anim_player.pause();
    m_machine_active_sprite = std::make_shared<sf::RectangleShape>();
    m_machine_active_sprite->setSize({55, 25});
    m_machine_active_sprite->setPosition(243, 19);
    m_machine_active_sprite->setTexture(&ns::Res::getTexture("machine_activate.png"));
    m_machine_active_sprite->setTextureRect(m_machine_active_anim_player.getActiveFrame().rectangle);

    game->scene->getLayer("back")->add(m_map->getTileLayer("back"));
    game->scene->getLayer("back")->add(m_map->getTileLayer("back2"));
    game->scene->getLayer("front")->add(m_map->getTileLayer("front"));
    game->scene->getLayer("entities")->add(game->player);
    game->scene->getLayer("top")->add(m_map->getTileLayer("top"));
    game->scene->getLayer("top")->add(m_map->getTileLayer("top2"));
    game->scene->getLayer("top")->add(m_machine_active_sprite);
    game->scene->getLayer("shapes")->add(m_map->getObjectLayer("collisions"));

    m_textbox = std::make_shared<TextBox>("Secret warehouse", game->fonts["default"]);
    game->ui_scene->getDefaultLayer()->add(m_textbox);

    game->camera->follow(game->player.get());
    game->camera->setLimitsRectangle(ns::IntRect(m_map->getTileLayer("back")->getGlobalBounds()));

    LevelState::init();
}

void WarehouseLevelState::onEvent(const sf::Event& event) {
    LevelState::onEvent(event);
}

void WarehouseLevelState::update() {
    m_machine_active_sprite->setTextureRect(m_machine_active_anim_player.getActiveFrame().rectangle);

    updateMap();
    updateTextbox();

    // checking for warp zone
    if (ns::FloatRect(262, 70, 18, 17).contains(game->player->collider()->getCollision().getShape().getGlobalBounds())) {
        game->player->inputs()->setCaptureInput(false);
        game->player->graphics<ns::ecs::SpriteComponent>(0)->setAnimState("idle");
        if (game->camera->getTop() > 0)
            game->camera->move(0, -1);
        game->camera->follow(nullptr);

        if (ns::Transition::list.empty()) {
            if (game->camera->getShader() == nullptr)
                m_machine_active_anim_player.resume();
        }
        m_machine_active_anim_player.update();
        if (!m_machine_active_anim_player.isPlaying())  {
            if (game->camera->getShader() == nullptr) {
                game->camera->toggleShader();
                auto* tr = new WaveOutTransition();
                tr->start();
                tr->setOnEndCallback([&](){
                    game->setState<LevelState>("egypt_out.tmx");
                    auto* tr = new WaveInTransition();
                    tr->start();
                    tr->setOnEndCallback([&](){
                        game->camera->getShader()->setUniform("time", 0.f);
                        game->camera->toggleShader();
                    });
                });
            }
        }
    }

    game->player->update();

}
