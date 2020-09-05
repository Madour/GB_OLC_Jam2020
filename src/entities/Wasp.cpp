// Created by Modar Nasser on 03/09/2020.

#include "entities/Wasp.hpp"
#include "Game.hpp"

Wasp::Wasp() : Enemy("Wasp") {
    auto* spritesheet = new ns::Spritesheet("wasp", ns::Res::getTexture("wasp.png"), {
        new ns::Anim("idle", {
            ns::AnimFrame({0, 0, 7, 10}, 100, {3, 9}),
            ns::AnimFrame({7, 0, 7, 10}, 100, {3, 9})
        })
    });
    m_spritesheet = std::unique_ptr<ns::Spritesheet>(spritesheet);
    addComponent<ns::ecs::SpriteComponent>(this, spritesheet, "idle");

    addComponent<ns::ecs::PhysicsComponent>(this, 1.f, sf::Vector2f(1.5, 1.5), sf::Vector2f(0.06, 0.06), sf::Vector2f(0.1, 0.1));

    addComponent<ns::ecs::ColliderComponent>(this, new ns::ecs::RectangleCollision(2, 2), sf::Vector2f(3, -2));

}

void Wasp::update() {
    if (ns::Transition::list.empty() && !game->isTextboxOpened())
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
            if (ns::distance(getPosition(), game->player->getPosition()) < 50) {
                m_target_locked = true;
                physics()->setDirection(dx / std::abs(dx), physics()->getDirection().y);
                physics()->setDirection(physics()->getDirection().x, dy / std::abs(dy));
            }
        }
        else {
            if (ns::distance(getPosition(), game->player->getPosition()) < 150 ) {
                physics()->setDirection(dx/std::abs(dx), physics()->getDirection().y);
                physics()->setDirection(physics()->getDirection().x, dy/std::abs(dy));
                if(std::abs(dx) < 32 && std::abs(dy) < 32) {
                    physics()->setMaxVelocity(2.5, 2.5);
                }
                else {
                    physics()->setMaxVelocity(1.5 , 1.5);
                };
            }
            else {
                m_target_locked = false;
                physics()->setDirection(0, 0);
            }
        }
    }
    else {
        physics()->setDirection(dx / std::abs(dx), physics()->getDirection().y);
        physics()->setDirection(physics()->getDirection().x, dy / std::abs(dy));
        m_target_locked = false;
    }

    collider()->update();

    if (collider()->getCollision().collide(game->player->collider()->getCollision())) {
        if (!game->isTextboxOpened())
            if (std::rand()%50 == 0)
                game->player->damage();
    }

    if (physics()->getDirection().x == 1)
        graphics<ns::ecs::SpriteComponent>(0)->getDrawable().setScale(-1, 1);
    else
        graphics<ns::ecs::SpriteComponent>(0)->getDrawable().setScale(1, 1);

    for (const auto& graphic_comp: graphics()) {
        graphic_comp->update();
    }
}
