// Created by Modar Nasser on 01/09/2020.

#include "Game.hpp"
#include "states/LevelState.hpp"
#include "MapCollisions.hpp"

LevelState::LevelState() = default;

LevelState::LevelState(const std::string& map_name, bool start_pos) {
    m_map_name = map_name;
    game->scene->clearAllLayers();
    game->ui_scene->getDefaultLayer()->clear();

    m_map = std::make_shared<ns::tm::TiledMap>();
    m_map->loadFromFile("assets/maps/"+map_name);

    m_map->getObjectLayer("collisions")->setVisible(false);

    m_spikes_texture.create(m_map->getSize().x*m_map->getTileSize().x, m_map->getSize().y*m_map->getTileSize().y);
    m_spikes_sprite = std::make_shared<sf::RectangleShape>(sf::Vector2f(m_map->getSize().x*m_map->getTileSize().x, m_map->getSize().y*m_map->getTileSize().y));
    for (int x = 0; x < m_map->getSize().x; ++x) {
        for (int y = 0; y < m_map->getSize().y; ++y) {
            auto& tile = m_map->getTileLayer("front")->getTile(x, y);
            auto& tileset = m_map->getTileTileset(tile.gid);
            if (tileset->name == "tileset" && tile.gid - tileset->firstgid == 44) {
                m_spikes.emplace_back(x*tileset->tilewidth, y*tileset->tileheight);
            }
        }
    }

    m_spikes_vertices.resize(m_spikes.size()*4);
    m_spikes_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
    m_spikes_texture.clear(sf::Color::Transparent);
    for (int i = 0; i < m_spikes_vertices.getVertexCount(); i+=4) {
        for (int j = 0; j < 4; ++j)
            m_spikes_vertices[i+j] = m_spikes[i/4].getVertices()[j];
    }
    m_spikes_texture.draw(m_spikes_vertices, sf::RenderStates(&ns::Res::getTexture("tileset.png")));
    m_spikes_texture.display();
    m_spikes_sprite->setTexture(&m_spikes_texture.getTexture());



    if (start_pos)
        game->player->setPosition(m_map->getProperty<float>("start_x"), m_map->getProperty<float>("start_y"));
    game->player->inputs()->setCaptureInput(true);

    game->scene->getLayer("back")->add(m_map->getTileLayer("back"));
    game->scene->getLayer("back")->add(m_map->getTileLayer("back2"));
    game->scene->getLayer("front")->add(m_map->getTileLayer("front"));
    game->scene->getLayer("front")->add(m_spikes_sprite);
    game->scene->getLayer("entities")->add(game->player);
    game->scene->getLayer("top")->add(m_map->getTileLayer("top"));
    game->scene->getLayer("top")->add(m_map->getTileLayer("top2"));
    game->scene->getLayer("shapes")->add(m_map->getObjectLayer("collisions"));

    game->camera->follow(game->player.get());
    game->camera->setLimitsRectangle(ns::IntRect(m_map->getTileLayer("back")->getGlobalBounds()));


}

void LevelState::init() {
    configurePalette();

    m_map->getObjectLayer("collisions")->setVisible(false);
    m_map->getObjectLayer("warpzone")->setVisible(false);

    if(m_map->hasProperty("show_hud"))
        game->hud->open();
    else
        game->hud->close();

    // storing map collisions
    MapCollisions::clear();
    for (auto& rect : m_map->getObjectLayer("collisions")->getRectangles()) {
        MapCollisions::add(ns::FloatRect(rect.getShape().getGlobalBounds()));
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
    m_spikes_texture.draw(m_spikes_vertices, sf::RenderStates(&ns::Res::getTexture("maps/tilesets/tileset.png")));
    m_spikes_texture.display();
    m_spikes_sprite->setTexture(&m_spikes_texture.getTexture());

    game->player->update();

    for (auto& spike : m_spikes) {
        if (spike.isUp())
            if (game->player->collider()->getCollision().getShape().getGlobalBounds().intersects(spike.getGlobalBounds()))
                game->player->damage();
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
    m_map->getTileLayer("front")->update();
    m_map->getTileLayer("top")->update();
    if (m_map_name == "egypt_out.tmx")
        m_map->getTileLayer("back2")->move(0.05, 0);

    auto player_box = game->player->collider()->getCollision().getShape().getGlobalBounds();

    for (auto& rect : m_map->getObjectLayer("warpzone")->getRectangles()) {
        ns::FloatRect zone{rect.getShape().getGlobalBounds()};
        if (zone.contains(player_box)) {
            game->player->inputs()->setCaptureInput(false);
            if (ns::Transition::list.empty()) {
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

    auto& textboxes = m_map->getObjectLayer("textboxes")->getRectangles();
    for (auto it = textboxes.begin(); it != textboxes.end(); ++it) {
        auto& rect = *it;
        if (rect.getShape().getGlobalBounds().intersects(player_box)) {
            std::string font = "default";
            if (rect.hasProperty("font")) {
                font = rect.getProperty<std::string>("font");
            }
            m_textbox = std::make_shared<TextBox>(rect.getProperty<std::string>("text"), game->fonts[font]);
            game->ui_scene->getDefaultLayer()->add(m_textbox);

            textboxes.erase(it--);
        }
    }

}