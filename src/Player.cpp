// Created by Modar Nasser on 29/08/2020.


#include "Player.hpp"

Player::Player() : ns::BaseEntity("Player") {
    auto* spr = new ns::Spritesheet("player", ns::Res::getTexture("pokemon.png"), {
        new ns::Anim("idle", {ns::AnimFrame({0, 0, 16, 16}, 1000, {8, 16})}, false),
        new ns::Anim("walk_down", {
            ns::AnimFrame({0, 0, 16, 16}, 150, {8, 16}),
            ns::AnimFrame({16, 0, 16, 16}, 150, {8, 16}),
            ns::AnimFrame({32, 0, 16, 16}, 150, {8, 16}),
            ns::AnimFrame({48, 0, 16, 16}, 150, {8, 16})
        }),
        new ns::Anim("walk_left", {
            ns::AnimFrame({0, 16, 16, 16}, 150, {8, 16}),
            ns::AnimFrame({16, 16, 16, 16}, 150, {8, 16}),
            ns::AnimFrame({32, 16, 16, 16}, 150, {8, 16}),
            ns::AnimFrame({48, 16, 16, 16}, 150, {8, 16}),
        }),
        new ns::Anim("walk_right", {
            ns::AnimFrame({0, 32, 16, 16}, 150, {8, 16}),
            ns::AnimFrame({16, 32, 16, 16}, 150, {8, 16}),
            ns::AnimFrame({32, 32, 16, 16}, 150, {8, 16}),
            ns::AnimFrame({48, 32, 16, 16}, 150, {8, 16})
        }),
        new ns::Anim("walk_up", {
            ns::AnimFrame({0, 48, 16, 16}, 150, {8, 16}),
            ns::AnimFrame({16, 48, 16, 16}, 150, {8, 16}),
            ns::AnimFrame({32, 48, 16, 16}, 150, {8, 16}),
            ns::AnimFrame({48, 48, 16, 16}, 150, {8, 16})
        })
    });
    m_spritesheet = std::unique_ptr<ns::Spritesheet>(spr);

    addComponent<ns::ecs::SpriteComponent>(this, m_spritesheet.get(), "idle");

    auto* ph = new ns::ecs::PhysicsComponent(this, 1.f, {0.75f, 0.75f}, {1.f, 1.f}, {1.f, 1.f});
    addComponent<ns::ecs::PhysicsComponent>(std::shared_ptr<ns::ecs::PhysicsComponent>(ph));

    addComponent<ns::ecs::InputsComponent>(this);
    inputs()->bind<Player>(sf::Keyboard::Up, &Player::moveUp);
    inputs()->bind<Player>(sf::Keyboard::Down, &Player::moveDown);
    inputs()->bind<Player>(sf::Keyboard::Left, &Player::moveLeft);
    inputs()->bind<Player>(sf::Keyboard::Right, &Player::moveRight);

    addComponent<ns::ecs::ColliderComponent>(this, new ns::ecs::RectangleCollision(14, 10), sf::Vector2f(1, -5));
}

void Player::moveUp() {
    if (physics()->getDirection().y != 1) {
        physics()->setDirection(physics()->getDirection().x, -1);
    }
}

void Player::moveDown() {
    if (physics()->getDirection().y != -1) {
        physics()->setDirection(physics()->getDirection().x, 1);
    }
}

void Player::moveLeft() {
    if (physics()->getDirection().x != 1) {
        physics()->setDirection(-1, physics()->getDirection().y);
    }
}

void Player::moveRight() {
    if (physics()->getDirection().x != -1) {
        physics()->setDirection(1, physics()->getDirection().y);
    }
}

void Player::update() {

    physics()->setDirection(0, 0);
    inputs()->update<Player>();
    if(physics()) physics()->update();
    if(collider()) collider()->update();

    auto col = collider()->getCollision().getShape().getGlobalBounds();
    for (const auto& rect : MapCollisions::all()) {
        sf::FloatRect intersection;
        if (col.intersects(rect, intersection)) {
            if (intersection.width < intersection.height) {
                setX(getX() + intersection.width * (physics()->getDirection().x == 0 ? 1.f : -1.f*physics()->getDirection().x));
            }
            else {
                // player over box
                if (std::abs(col.top - rect.bottom()) >= std::abs(col.top+col.height - rect.top)) {
                    setY(getY() - intersection.height);
                }
                else if (std::abs(col.top - rect.bottom()) < std::abs(col.top+col.height - rect.top)){
                    setY(getY() + intersection.height);
                }
                else {
                }

            }
        }
    }

    if (physics()->getDirection() == sf::Vector2i(0, 0))
        graphics<ns::ecs::SpriteComponent>(0)->getAnimPlayer().stop();
    else {
        if (physics()->getDirection().x == 0) {
            if (physics()->getDirection().y == -1)
                graphics<ns::ecs::SpriteComponent>(0)->setAnimState("walk_up");
            else
                graphics<ns::ecs::SpriteComponent>(0)->setAnimState("walk_down");
        }
        if (physics()->getDirection().y == 0) {
            if (physics()->getDirection().x == -1)
                graphics<ns::ecs::SpriteComponent>(0)->setAnimState("walk_left");
            else
                graphics<ns::ecs::SpriteComponent>(0)->setAnimState("walk_right");
        }
    }
    for (const auto& graphic_comp: graphics()) {
        graphic_comp->update();
    }
}
