// Created by Modar Nasser on 06/09/2020.

#include "Game.hpp"
#include "entities/Enemy.hpp"
#include "states/EndGameState.hpp"

void EndGameState::init() {
    game->scene->clearAllLayers();
    game->ui_scene->getDefaultLayer()->clear();
    Enemy::list.clear();
    game->player->getItems()[0] = Item(ItemType::PresentButton);
    game->player->getItems()[1] = Item(ItemType::None);
    game->player->getItems()[2] = Item(ItemType::None);
    game->hud->resetTimer(5*60);

    m_ccl = std::make_shared<ns::BitmapText>("Unfortunately, you were not able to collect enough artifacts before the end of the week. "
                                             "\nWith no money in your pocket, you had no choice but to close the museum. "
                                             "\n\nYou decided to detroy the Great Machine and leave no evidence of its existence. "
                                             "\n\n\n\n Thank you for playing !");
    m_ccl->setFont(game->fonts["default"]);
    m_ccl->setMaxWidth(140);
    m_ccl->setPosition(10, 160);
    game->ui_scene->getDefaultLayer()->add(m_ccl);

    auto bg = std::make_shared<sf::RectangleShape>(sf::Vector2f(160, 96));
    bg->setFillColor(sf::Color::Black);
    game->ui_scene->getDefaultLayer()->add(bg);

    auto t = std::make_shared<ns::BitmapText>("Results");
    t->setFont(game->fonts["bold"]);
    t->setPosition(80 - t->getWidth()/2, 20);
    game->ui_scene->getDefaultLayer()->add(t);

    for (int i = 0 ; i < m_artifacts.size(); ++i) {
        m_artifacts[i] = std::make_shared<Artifact>((ArtifactType)(i+1));
        m_artifacts[i]->setPosition(32+32*i, 54);
        m_artifacts[i]->getSprite().setColor(sf::Color::Transparent);
        game->ui_scene->getDefaultLayer()->add(m_artifacts[i]);
    }
    for (int i = 0; i < m_artifacts.size(); ++i) {
        m_checkboxes[i] = std::make_shared<sf::RectangleShape>(sf::Vector2f(14, 14));
        m_checkboxes[i]->setPosition(25+32*i, 54+16);
        m_checkboxes[i]->setTexture(&ns::Res::getTexture("gui.png"));
        if (i == 1)
            m_checkboxes[i]->setTextureRect({80, 20, 14, 14});
        else
            m_checkboxes[i]->setTextureRect({80+16, 20, 14, 14});

        m_checkboxes[i]->setFillColor(sf::Color::Transparent);
        game->ui_scene->getDefaultLayer()->add(m_checkboxes[i]);
    }

}

void EndGameState::onEvent(const sf::Event& event) {
    /*if (ticks > 500 && event.type == sf::Event::KeyPressed)
        if (event.key.code == ns::Config::Inputs::getButtonKey("start")) {
            auto* tr = new PaletteShiftOutTransition();
            tr->start();
            tr->setOnEndCallback([](){
                (new ns::transition::CircleOpen())->start();
                game->setState<LevelState>(game->last_map_name);
                game->player->setHP(1);
                game->player->restoreHp(9);
                game->hud->resetTimer(5*60);
            });
        }*/
}

void EndGameState::update() {
    if (ns::Transition::list.empty()) {
        for (int i = 0 ; i < m_artifacts.size(); ++i) {
            auto& item = m_artifacts[i];
            if (item->getSprite().getColor() == sf::Color::Transparent) {
                if (ticks > (i+1)*40) {
                    game->playSound("tsh");
                    item->getSprite().setColor(sf::Color::White);
                    m_checkboxes[i]->setFillColor(sf::Color::White);
                }
            }
            else {
                item->update();
            }
        }
        if (ticks % 9 == 0 && m_ccl->getPosition().y > -28)
            m_ccl->move(0, -1);
        ticks ++;
    }
}
