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

    addComponent<ns::ecs::ColliderComponent>(this, new ns::ecs::RectangleCollision(12, 8), sf::Vector2f(0, -4));
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

    if (ns::Transition::list.empty())
        inputs()->update<Player>();

    physics()->update();
    collider()->update();

    // resolve collisions
    sf::FloatRect intersection;
    for (const auto& rect : MapCollisions::all()) {
        auto col = ns::FloatRect(collider()->getCollision().getShape().getGlobalBounds());
        if (col.intersects(rect, intersection)) {
            if (rect.width == 1 && physics()->getDirection() == sf::Vector2i(0, -1)) {
                if (std::abs(col.left - rect.left) > std::abs(col.right() - rect.right())) {
                    setX(rect.left - col.width/2);
                }
                else if (std::abs(col.left - rect.left) < std::abs(col.right() - rect.right())) {
                    setX(rect.right() + col.width/2);
                }
            }
            else {
                if (intersection.width < intersection.height && intersection.height >= 3) {
                    // player on left side of box
                    if (std::abs(col.left - rect.right()) >= std::abs(col.right() - rect.left))
                        setX(getX() - intersection.width);
                        // player right side of box
                    else
                        setX(getX() + intersection.width);
                }
                else if (intersection.width >= 3) {
                    // player over box
                    if (std::abs(col.top - rect.bottom()) >= std::abs(col.bottom() - rect.top))
                        setY(getY() - intersection.height);
                    // player under box
                    else
                        setY(getY() + intersection.height);
                }
            }
            collider()->update();
        }
    }

    // animate player
    if (physics()->getDirection() == sf::Vector2i(0, 0))
        graphics<ns::ecs::SpriteComponent>(0)->getAnimPlayer().stop();
    else {
        if (physics()->getDirection().x == 0) {
            if (physics()->getDirection().y == -1)
                graphics<ns::ecs::SpriteComponent>(0)->setAnimState("walk_up");
            else
                graphics<ns::ecs::SpriteComponent>(0)->setAnimState("walk_down");
        }
        else if (physics()->getDirection().y == 0){
            if (physics()->getDirection().x == -1)
                graphics<ns::ecs::SpriteComponent>(0)->setAnimState("walk_left");
            else
                graphics<ns::ecs::SpriteComponent>(0)->setAnimState("walk_right");
        }

        // dirty code, handles bug when move button pressed before textbox closed
        if (graphics<ns::ecs::SpriteComponent>(0)->getAnimState() == "idle") {
            if (physics()->getDirection().y == -1)
                graphics<ns::ecs::SpriteComponent>(0)->setAnimState("walk_up");
            else if (physics()->getDirection().y == 1)
                graphics<ns::ecs::SpriteComponent>(0)->setAnimState("walk_down");
            else if (physics()->getDirection().x == -1)
                graphics<ns::ecs::SpriteComponent>(0)->setAnimState("walk_left");
            else
                graphics<ns::ecs::SpriteComponent>(0)->setAnimState("walk_right");
        }
    }

    for (const auto& graphic_comp: graphics()) {
        graphic_comp->update();
    }
}
