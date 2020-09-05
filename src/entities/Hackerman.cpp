// Created by Modar Nasser on 05/09/2020.

#include "entities/Hackerman.hpp"
#include "MapCollisions.hpp"
#include "Game.hpp"

Hackerman::Hackerman() : BaseEntity("Hackerman") {
    auto* spritesheet = new ns::Spritesheet("player", ns::Res::getTexture("hackerman.png"), {
        new ns::Anim("idle", {ns::AnimFrame({0, 0, 11, 22}, 1000, {5, 22})}, false),

        new ns::Anim("walk_down", {
            ns::AnimFrame({0, 0, 11, 22}, 150, {5, 22}),
            ns::AnimFrame({0, 22, 11, 22}, 150, {5, 22}),
            ns::AnimFrame({0, 0, 11, 22}, 150, {5, 22}),
            ns::AnimFrame({0, 44, 11, 22}, 150, {5, 22})
        }),

        new ns::Anim("walk_up", {
            ns::AnimFrame({11, 0, 11, 22}, 150, {5, 22}),
            ns::AnimFrame({11, 22, 11, 22}, 150, {5, 22}),
            ns::AnimFrame({11, 0, 11, 22}, 150, {5, 22}),
            ns::AnimFrame({11, 44, 11, 22}, 150, {5, 22})
        }),

        new ns::Anim("walk_side", {
            ns::AnimFrame({22, 0, 11, 22}, 150, {4, 22}),
            ns::AnimFrame({22, 22, 11, 22}, 150, {4, 22}),
            ns::AnimFrame({22, 0, 11, 22}, 150, {4, 22}),
            ns::AnimFrame({22, 44, 11, 22}, 150, {4, 22})
        }),

        new ns::Anim("sit", {
            ns::AnimFrame({33, 0, 11, 22}, 200, {5, 22}),
            ns::AnimFrame({33, 22, 11, 22}, 200, {5, 22}),
        })
    });

    m_spritesheet = std::unique_ptr<ns::Spritesheet>(spritesheet);
    addComponent<ns::ecs::SpriteComponent>(this, spritesheet, "idle");

    addComponent<ns::ecs::ColliderComponent>(this, new ns::ecs::RectangleCollision(5, 5), sf::Vector2f(0, -3));

    addComponent<ns::ecs::PhysicsComponent>(this, 1.f, sf::Vector2f(1, 1), sf::Vector2f(1, 1), sf::Vector2f(1, 1));
}

void Hackerman::setDestination(float x, float y) {
    m_destination = {x, y};
}

void Hackerman::update() {
    physics()->setDirection(0, 0);

    auto sprite = graphics<ns::ecs::SpriteComponent>(0);
    if (sprite->getAnimState() == "sit")
        m_destination = getPosition();
    auto d = sf::Vector2i(m_destination.x - std::round(getPosition().x), m_destination.y - std::round(getPosition().y));
    if (std::abs(d.x) > 1)
        physics()->setDirection(d.x/std::abs(d.x), physics()->getDirection().y);
    else
        m_destination.x = getPosition().x;
    if (std::abs(d.y) > 1)
        physics()->setDirection(physics()->getDirection().x, d.y/std::abs(d.y));
    else
        m_destination.y = getPosition().y;

    if (ns::Transition::list.empty()) {
        physics()->update();
    }
    collider()->update();

    if (physics()->getDirection() == sf::Vector2i(0, 0))
        sprite->getAnimPlayer().stop();
    else {
        if (std::abs(d.y) > std::abs(d.x)) {
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