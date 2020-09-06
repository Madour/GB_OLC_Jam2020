// Created by Modar Nasser on 01/09/2020.

#include "Game.hpp"
#include "states/LevelState.hpp"
#include "states/GameOverState.hpp"
#include "MapCollisions.hpp"

LevelState::LevelState() = default;

LevelState::~LevelState() {
    Enemy::list.clear();
}

LevelState::LevelState(const std::string& map_name, bool start_pos) {
    m_map_name = map_name;
    game->scene->clearAllLayers();
    game->ui_scene->getDefaultLayer()->clear();

    m_map = std::make_shared<ns::tm::TiledMap>();
    m_map->loadFromFile("assets/maps/"+map_name);

    m_map->getObjectLayer("collisions")->setVisible(false);

    // loading spikes
    m_spikes_texture.create(m_map->getSize().x*m_map->getTileSize().x, m_map->getSize().y*m_map->getTileSize().y);
    m_spikes_sprite = std::make_shared<sf::RectangleShape>(sf::Vector2f(m_map->getSize().x*m_map->getTileSize().x, m_map->getSize().y*m_map->getTileSize().y));
    for (int x = 0; x < m_map->getSize().x; ++x) {
        for (int y = 0; y < m_map->getSize().y; ++y) {
            auto& tile = m_map->getTileLayer("back2")->getTile(x, y);
            auto& tileset = m_map->getTileTileset(tile.gid);
            if (tileset->name == "tileset" && tile.gid - tileset->firstgid == 44) {
                m_spikes.emplace_back(x*tileset->tilewidth, y*tileset->tileheight);
            }
        }
    }
    tileset_texture = &ns::Res::get().in("maps").in("tilesets").getTexture("tileset.png");
    m_spikes_vertices.resize(m_spikes.size()*4);
    m_spikes_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
    m_spikes_texture.clear(sf::Color::Transparent);
    for (int i = 0; i < m_spikes_vertices.getVertexCount(); i+=4) {
        for (int j = 0; j < 4; ++j)
            m_spikes_vertices[i+j] = m_spikes[i/4].getVertices()[j];
    }
    m_spikes_texture.draw(m_spikes_vertices, sf::RenderStates(tileset_texture));
    m_spikes_texture.display();
    m_spikes_sprite->setTexture(&m_spikes_texture.getTexture());

    // spawning enemies
    if (m_map->hasLayer("spawns"))
        for (auto& pt_obj : m_map->getObjectLayer("spawns")->getPoints()) {
            Enemy::createFromName(pt_obj.getProperty<std::string>("type"))->setPosition(pt_obj.getShape().getPosition());
        }

    if (start_pos)
        game->player->setPosition(m_map->getProperty<float>("start_x"), m_map->getProperty<float>("start_y"));
    game->player->inputs()->setCaptureInput(true);

    game->scene->getLayer("back")->add(m_map->getTileLayer("back"));
    game->scene->getLayer("back")->add(m_map->getTileLayer("back2"));
    game->scene->getLayer("front")->add(m_map->getTileLayer("front"));
    game->scene->getLayer("front")->add(m_spikes_sprite);
    game->scene->getLayer("entities")->add(game->player);
    for (auto& e : Enemy::list)
        game->scene->getLayer("entities")->add(e);

    game->scene->getLayer("top")->add(m_map->getTileLayer("top"));
    game->scene->getLayer("top")->add(m_map->getTileLayer("top2"));
    game->scene->getLayer("shapes")->add(m_map->getObjectLayer("collisions"));

    game->camera->follow(game->player.get());
    game->camera->setLimitsRectangle(ns::IntRect(m_map->getTileLayer("back")->getGlobalBounds()));

}

void LevelState::init() {
    configurePalette();

    m_map->getObjectLayer("collisions")->setVisible(false);

    if(m_map->hasProperty("show_hud") && m_map->getProperty<bool>("show_hud"))
        game->hud->open();
    else
        game->hud->close();

    if (m_map->hasProperty("music")) {
        game->playMusic(m_map->getProperty<std::string>("music"));
    }

    if (m_map->hasProperty("reset_timer")) {
        game->hud->resetTimer(m_map->getProperty<int>("reset_timer"));
    }

    if (m_map->hasLayer("artifact")) {
        auto& obj = m_map->getObjectLayer("artifact")->getPoints()[0];
        m_artifact = std::make_shared<Artifact>((ArtifactType)obj.getProperty<int>("type"));
        m_artifact->setPosition(obj.getShape().getPosition().x, obj.getShape().getPosition().y);
        game->scene->getLayer("entities")->add(m_artifact);
    }

    // storing map collisions
    MapCollisions::clear();
    for (auto& rect : m_map->getObjectLayer("collisions")->getRectangles()) {
        MapCollisions::add(ns::FloatRect(rect.getShape().getGlobalBounds()));
    }

    for (auto& enemy : Enemy::list) {
        if (enemy->targetLocked())
            enemy->setPosition(game->player->getPosition() - enemy->getDistanceToTarget());
        if (enemy->getName() == "Wasp")
            game->scene->getLayer("top")->add(enemy);
        else
            game->scene->getLayer("entities")->add(enemy);
    }
}


void LevelState::onEvent(const sf::Event& event) {
    m_map->getObjectLayer("collisions")->setVisible(ns::Config::debug);
    if (m_textbox)
        m_textbox->onEvent(event);
}

void LevelState::update() {
    updateMap();
    updateTextbox();
    m_spikes_texture.clear(sf::Color::Transparent);
    for (int i = 0; i < m_spikes_vertices.getVertexCount(); i+=4) {
        m_spikes[i/4].update();
        for (int j = 0; j < 4; ++j) {
            m_spikes_vertices[i+j] = m_spikes[i/4].getVertices()[j];
        }
    }
    m_spikes_texture.draw(m_spikes_vertices, sf::RenderStates(tileset_texture));
    m_spikes_texture.display();
    m_spikes_sprite->setTexture(&m_spikes_texture.getTexture());

    if (m_artifact != nullptr)
        m_artifact->update();
    game->player->update();

    for (auto& spike : m_spikes) {
        if (spike.isUp())
            if (game->player->collider()->getCollision().getShape().getGlobalBounds().intersects(spike.getGlobalBounds()))
                game->player->damage();
    }

    if (game->hud->getRemainingTime() <= 0 && ns::Transition::list.empty()) {
        auto* tr = new PaletteShiftOutTransition();
        tr->start();
        tr->setOnEndCallback([](){
            (new PaletteShiftInTransition())->start();
            game->setState<GameOverState>();
        });
    }
}

void LevelState::configurePalette() {
    game->setPalette(m_map->getProperty<int>("palette"));
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
    if (m_map->hasLayer("machine_top"))
        m_map->getTileLayer("machine_top")->update();
    m_map->getTileLayer("top")->update();
    m_map->getTileLayer("front")->update();
    m_map->getTileLayer("back2")->update();
    m_map->getTileLayer("back")->update();
    if (m_map_name == "egypt_out.tmx")
        m_map->getTileLayer("back2")->move(0.05, 0);

    auto player_box = game->player->collider()->getCollision().getShape().getGlobalBounds();

    if (m_map->hasLayer("warpzone"))
        for (auto& rect : m_map->getObjectLayer("warpzone")->getRectangles()) {
            ns::FloatRect zone{rect.getShape().getGlobalBounds()};
            if (zone.contains(player_box)) {
                game->player->inputs()->setCaptureInput(false);
                if (ns::Transition::list.empty()) {
                    if (rect.hasProperty("fade_out") && rect.getProperty<bool>("fade_out"))
                        game->musicFadeOut();
                    auto* tr = new PaletteShiftOutTransition();
                    tr->start();
                    tr->setOnEndCallback([&]() {
                        if (rect.hasProperty("posx")) {
                            game->setState<LevelState>(rect.getProperty<std::string>("destination"), false);
                            game->player->setX(rect.getProperty<float>("posx"));
                            game->player->setY(rect.getProperty<float>("posy"));
                        } else
                            game->setState<LevelState>(rect.getProperty<std::string>("destination"));
                        auto* tr = new PaletteShiftInTransition();
                        tr->start();
                    });
                }
            }
        }

    if (m_map->hasLayer("textboxes") && m_textbox == nullptr) {
        auto& textboxes = m_map->getObjectLayer("textboxes")->getRectangles();
        for (auto it = textboxes.begin(); it != textboxes.end(); it++) {
            auto& rect = *it;
            if (rect.getShape().getGlobalBounds().contains(game->player->getPosition())) {
                std::string font = "default";
                std::string label;
                if (rect.hasProperty("font"))
                    font = rect.getProperty<std::string>("font");
                if (rect.hasProperty("label"))
                    label = rect.getProperty<std::string>("label");
                if (rect.hasProperty("text")) {
                    m_textbox = std::make_shared<TextBox>(rect.getProperty<std::string>("text"), game->fonts[font], label);
                    game->ui_scene->getDefaultLayer()->add(m_textbox);
                }

                if (rect.hasProperty("dest_x"))
                    game->player->setDestination(rect.getProperty<int>("dest_x"), rect.getProperty<int>("dest_y"));

                if (rect.hasProperty("music")) {
                    game->playMusic(rect.getProperty<std::string>("music"));
                }

                it = textboxes.erase(it);
                break;
            }
        }
    }

   if (m_map->hasLayer("interractions")){
        auto& interractions = m_map->getObjectLayer("interractions")->getRectangles();
        for (auto it = interractions.begin(); it != interractions.end(); it++) {
            auto& rect = *it;
            if (rect.getShape().getGlobalBounds().intersects(player_box)) {
                if (rect.hasProperty("dir_y"))
                    if (game->player->getFaceDirection().y != rect.getProperty<int>("dir_y"))
                        continue;

                if (rect.hasProperty("dir_x"))
                    if (game->player->getFaceDirection().x != rect.getProperty<int>("dir_x"))
                        continue;

                if ((rect.hasProperty("auto") && rect.getProperty<bool>("auto"))
                    || sf::Keyboard::isKeyPressed(ns::Config::Inputs::getButtonKey("A"))) {
                    m_textbox = std::make_shared<TextBox>(rect.getProperty<std::string>("text"), game->fonts["italic"]);
                    game->ui_scene->getDefaultLayer()->add(m_textbox);

                    if (rect.hasProperty("item"))
                        game->player->addItem((ItemType) rect.getProperty<int>("item"));
                    it = interractions.erase(it);
                    break;
                }
            }
        }
    }

}
