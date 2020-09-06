// Created by Modar Nasser on 03/09/2020.

#include "entities/BigMummy.hpp"
#include "Game.hpp"

BigMummy::BigMummy() : Enemy("BigMummy") {
    auto* spritesheet = new ns::Spritesheet("mummy_big", ns::Res::getTexture("mummy_big.png"), {
        new ns::Anim("idle", {ns::AnimFrame({0, 0, 13, 23}, 1000, {6, 23})}, false),
        new ns::Anim("walk_down", {ns::AnimFrame({0, 0, 13, 23}, 500, {6, 23}), ns::AnimFrame({13, 0, 13, 23}, 500, {6, 23})}),
        new ns::Anim("walk_up", {ns::AnimFrame({0, 23, 13, 23}, 500, {6, 23}), ns::AnimFrame({13, 23, 13, 23}, 500, {6, 23})}),
        new ns::Anim("walk_side", {ns::AnimFrame({0, 46, 13, 23}, 500, {6, 23}), ns::AnimFrame({13, 46, 13, 23}, 500, {6, 23})}),
    });
    m_spritesheet = std::unique_ptr<ns::Spritesheet>(spritesheet);
    addComponent<ns::ecs::SpriteComponent>(this, spritesheet, "idle");

    addComponent<ns::ecs::PhysicsComponent>(this, 1.f, sf::Vector2f(1, 1), sf::Vector2f(0.1, 0.1), sf::Vector2f(0.8, 0.8));

    addComponent<ns::ecs::ColliderComponent>(this, new ns::ecs::RectangleCollision(14, 8), sf::Vector2f(0, -4));

}

void BigMummy::update() {
    if (ns::Transition::list.empty() && !game->isTextboxOpened() && m_cooldown == 0)
        physics()->update();

    if (!game->player->isInvisible())
        m_target_pos = game->player->getPosition();
    else
        m_target_locked = false;

    m_distance_to_target = m_target_pos - getPosition();
    auto dx = (int)m_distance_to_target.x;
    auto dy = (int)m_distance_to_target.y;

    if (!game->player->isInvisible()) {
        if (!m_target_locked) {
            if (ns::distance(getPosition(), game->player->getPosition()) < 60) {
                m_target_locked = true;
                game->playSound("grr");
                if (dx != 0) physics()->setDirection(dx / std::abs(dx), physics()->getDirection().y);
                if (dy != 0) physics()->setDirection(physics()->getDirection().x, dy / std::abs(dy));
            }
            else {
                physics()->setDirection(0, 0);
            }
        }
        else {
            if (std::abs(dx) < 100 && std::abs(dy) < 100) {
                if (dx != 0) physics()->setDirection(dx / std::abs(dx), physics()->getDirection().y);
                if (dy != 0) physics()->setDirection(physics()->getDirection().x, dy / std::abs(dy));
                if (std::abs(dx) < 5)
                    physics()->setDirection(0, physics()->getDirection().y);
                if (std::abs(dy) < 5)
                    physics()->setDirection(physics()->getDirection().x, 0);
            }
            else {
                m_target_locked = false;
                physics()->setDirection(0, 0);
            }
        }
    }
    else  {
        m_target_locked = false;
        physics()->setDirection(0, 0);
    }

    collider()->update();

    if (collider()->getCollision().collide(game->player->collider()->getCollision())) {
        if (!game->isTextboxOpened()) {
            game->player->damage();
            m_cooldown = 30;
        }
    }
    if (m_cooldown > 0)
        m_cooldown--;
    // resolve collisions
    for (const auto& rect : MapCollisions::all()) {
        resolveCollision(this, rect);
    }
    for (const auto& ent : Enemy::list) {
        if (ent.get() != this && ent->getName() != "Wasp")
            resolveCollision(this, ns::FloatRect(ent->collider()->getCollision().getShape().getGlobalBounds()));
    }

    auto sprite = graphics<ns::ecs::SpriteComponent>(0);
    if (physics()->getDirection() == sf::Vector2i(0, 0))
        sprite->getAnimPlayer().stop();
    else {
        if (std::abs(dy) > std::abs(dx)) {
            if (physics()->getDirection().y == -1)
                sprite->setAnimState("walk_up");
            else
                sprite->setAnimState("walk_down");
        }
        else {
            sprite->setAnimState("walk_side");
            if (physics()->getDirection().x == -1)
                sprite->getDrawable().setScale(-1, 1);
            else
                sprite->getDrawable().setScale(1, 1);
        }
    }

    for (const auto& graphic_comp: graphics()) {
        graphic_comp->update();
    }
}
