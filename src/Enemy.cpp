// Created by Modar Nasser on 02/09/2020.

#include "Enemy.hpp"
#include "Game.hpp"

std::vector<Enemy*> Enemy::list;

Enemy::Enemy() : ns::BaseEntity("Enemy") {
    list.push_back(this);
    auto* shape_comp = new ns::ecs::ShapeComponent<sf::RectangleShape>(this, {16, 16});
    shape_comp->getDrawable().setOrigin(8, 16);
    shape_comp->getDrawable().setFillColor(sf::Color::Black);
    addComponent(std::shared_ptr<ns::ecs::ShapeComponent<sf::RectangleShape>>(shape_comp));

    addComponent<ns::ecs::PhysicsComponent>(this, 1.f, sf::Vector2f(0.8, 0.8), sf::Vector2f(0.02, 0.02), sf::Vector2f(0.1, 0.1));

    addComponent<ns::ecs::ColliderComponent>(this, new ns::ecs::RectangleCollision(14, 8), sf::Vector2f(0, -4));
}

void Enemy::update() {
    physics()->update();

    auto dx = game->player->getX() - getX();
    auto dy = game->player->getY() - getY();

    if (!m_target_locked) {
        if (std::abs(dx) < 32 && std::abs(dy) < 32) {
            m_target_locked = true;
            physics()->setDirection(dx / std::abs(dx), physics()->getDirection().y);
            physics()->setDirection(physics()->getDirection().x, dy / std::abs(dy));
        }
        else {
            physics()->setDirection(0, 0);
        }
    }
    else {
        if (std::abs(dx) < 100 && std::abs(dy) < 100) {
            physics()->setDirection(dx/std::abs(dx), physics()->getDirection().y);
            physics()->setDirection(physics()->getDirection().x, dy/std::abs(dy));
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

    collider()->update();

    // resolve collisions
    for (const auto& rect : MapCollisions::all()) {
        resolveCollision(this, rect);
    }
    for (const auto& ent : Enemy::list) {
        if (ent != this)
            resolveCollision(this, ns::FloatRect(ent->collider()->getCollision().getShape().getGlobalBounds()));
    }

    for (const auto& graphic_comp: graphics()) {
        graphic_comp->update();
    }
}
