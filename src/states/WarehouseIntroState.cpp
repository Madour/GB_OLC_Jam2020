// Created by Modar Nasser on 30/08/2020.

#include "Game.hpp"
#include "states/WarehouseIntroState.hpp"
#include "states/MuseumIntroState.hpp"
#include "states/EndGameState.hpp"

void WarehouseIntroState::init() {

    game->scene->clearAllLayers();
    game->ui_scene->getDefaultLayer()->clear();

    m_map = std::make_shared<ns::tm::TiledMap>();
    m_map->loadFromFile("assets/maps/warehouse_intro1.tmx");

    game->player->graphics<ns::ecs::SpriteComponent>(0)->setAnimState("idle");
    game->player->setPosition(120, 41);
    game->player->setDestination(65, 106);

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
    m_machine_active_sprite->setOrigin(0, 25);
    m_machine_active_sprite->setPosition(163, 45);
    m_machine_active_sprite->setTexture(&ns::Res::getTexture("machine_activate.png"));
    m_machine_active_sprite->setTextureRect(m_machine_active_anim_player.getActiveFrame().rectangle);

    m_black_screen = std::make_shared<sf::RectangleShape>(sf::Vector2f(160, 144));
    m_black_screen->setPosition(160, 0);
    m_black_screen->setFillColor(sf::Color::Black);

    m_splash_hackerman = std::make_shared<sf::RectangleShape>(sf::Vector2f(160, 144));
    m_splash_hackerman->setTexture(&ns::Res::getTexture("hackerman_artwork.png"));
    m_splash_hackerman->setPosition(160, 0);

    m_title_hackerman = std::make_shared<sf::RectangleShape>(sf::Vector2f(160, 144));
    m_title_hackerman->setTexture(&ns::Res::getTexture("hackerman_title.png"));
    m_title_hackerman->setPosition(200, 0);

    m_hackerman = std::make_shared<Hackerman>();
    m_hackerman->setPosition(120, 41);
    m_hackerman->setDestination(115, 75);

    game->scene->getLayer("back")->add(m_map->getTileLayer("back"));
    game->scene->getLayer("back")->add(m_map->getTileLayer("back2"));
    game->scene->getLayer("entities")->add(m_hackerman);
    game->scene->getLayer("entities")->add(game->player);
    game->scene->getLayer("top")->add(m_map->getTileLayer("top"));
    game->scene->getLayer("top")->add(m_map->getTileLayer("top2"));
    game->scene->getLayer("shapes")->add(m_map->getObjectLayer("collisions"));
    game->ui_scene->getDefaultLayer()->add(m_black_screen);
    game->ui_scene->getDefaultLayer()->add(m_splash_hackerman);
    game->ui_scene->getDefaultLayer()->add(m_title_hackerman);

    //m_textbox = std::make_shared<TextBox>("Secret warehouse", game->fonts["italic"]);
    //game->ui_scene->getDefaultLayer()->add(m_textbox);

    game->camera->follow(game->player.get());
    game->camera->setLimitsRectangle(ns::IntRect(m_map->getTileLayer("back")->getGlobalBounds()));
    game->camera->setFramesDelay(0);

    LevelState::init();
}

void WarehouseIntroState::onEvent(const sf::Event& event) {
    LevelState::onEvent(event);
}

void WarehouseIntroState::update() {
    m_machine_active_sprite->setTextureRect(m_machine_active_anim_player.getActiveFrame().rectangle);

    updateMap();
    updateTextbox();

    if (game->player->getX() >= 80 && game->player->getY() >= 105 && game->ui_scene->getDefaultLayer()->getDrawables().size() > 1) {
        if (m_black_screen->getPosition().x > 0 || m_splash_hackerman->getPosition().x < -50)
            m_black_screen->move(-2, 0);
        if (m_splash_hackerman->getPosition().x > 80 || m_splash_hackerman->getPosition().x <= 0)
            m_splash_hackerman->move(-2, 0);
        else
            m_splash_hackerman->move(-1, 0);
        if (m_title_hackerman->getPosition().x > 70 || m_title_hackerman->getPosition().x <= 0)
            m_title_hackerman->move(-2, 0);
        else
            m_title_hackerman->move(-1, 0);

        if (m_black_screen->getPosition().x <= 1) {
            if (game->scene->getLayer("front")->getDrawables().empty())
                game->scene->getLayer("front")->add(m_map->getTileLayer("front"));
            if (game->scene->getLayer("top")->getDrawables().size() == 2) {
                game->scene->getLayer("top")->add(m_map->getTileLayer("machine_top"));
                game->scene->getLayer("top")->add(m_machine_active_sprite);
            }
            m_hackerman->setPosition(165, 113);
            m_hackerman->setDestination(165, 113);
            m_hackerman->graphics<ns::ecs::SpriteComponent>(0)->setAnimState("walk_up");
            m_hackerman->graphics<ns::ecs::SpriteComponent>(0)->getAnimPlayer().stop();

            game->player->setPosition(192, 118);
            game->player->setDestination(192, 118);
            game->player->graphics<ns::ecs::SpriteComponent>(0)->setAnimState("walk_up");
            game->player->graphics<ns::ecs::SpriteComponent>(0)->getAnimPlayer().stop();

        }
        if (m_black_screen->getPosition().x <= -160) {
            game->ui_scene->getDefaultLayer()->clear();
            game->musicFadeOut();
        }
    }

    if (m_map->getObjectLayer("textboxes")->getRectangles().size() <= 0 ) {
        m_hackerman->physics()->setDirection(0, 0);
        m_hackerman->setY(m_hackerman->getY() - 5);
        m_hackerman->setDestination(m_hackerman->getPosition().x, m_hackerman->getPosition().y);
    }

    // checking for warp zone
    if (ns::FloatRect(180, 70, 24, 21).contains(game->player->collider()->getCollision().getShape().getGlobalBounds())) {
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
                    auto level = Game::travels.front();
                    Game::travels.pop_front();
                    if (!level.empty())
                        game->setState<LevelState>(level);
                    else
                        game->setState<EndGameState>();
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
    m_hackerman->update();

}
