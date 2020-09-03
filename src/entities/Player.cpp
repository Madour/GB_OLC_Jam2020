// Created by Modar Nasser on 29/08/2020.


#include "entities/Player.hpp"
#include "Game.hpp"

Player::Player() : ns::BaseEntity("Player") {
    auto* spr = new ns::Spritesheet("player", ns::Res::getTexture("player.png"), {
        new ns::Anim("idle", {ns::AnimFrame({0, 0, 13, 21}, 1000, {6, 21})}, false),
        new ns::Anim("walk_down", {
            ns::AnimFrame({0, 0, 13, 21}, 150, {6, 21}),
            ns::AnimFrame({13, 0, 13, 21}, 150, {6, 21}),
            ns::AnimFrame({0, 0, 13, 21}, 150, {6, 21}),
            ns::AnimFrame({26, 0, 13, 21}, 150, {6, 21})
        }),
        new ns::Anim("walk_side", {
            ns::AnimFrame({0, 21, 13, 21}, 150, {6, 21}),
            ns::AnimFrame({13, 21, 13, 21}, 150, {6, 21}),
            ns::AnimFrame({0, 21, 13, 21}, 150, {6, 21}),
            ns::AnimFrame({26, 21, 13, 21}, 150, {6, 21}),
        }),
        new ns::Anim("walk_up", {
            ns::AnimFrame({0, 42, 13, 21}, 150, {6, 21}),
            ns::AnimFrame({13, 42, 13, 21}, 150, {6, 21}),
            ns::AnimFrame({0, 42, 13, 21}, 150, {6, 21}),
            ns::AnimFrame({26, 42, 13, 21}, 150, {6, 21})
        })
    });
    m_spritesheet = std::unique_ptr<ns::Spritesheet>(spr);

    addComponent<ns::ecs::SpriteComponent>(this, m_spritesheet.get(), "idle");

    auto* ph = new ns::ecs::PhysicsComponent(this, 1.f, {0.9f, 0.9f}, {1.f, 1.f}, {1.f, 1.f});
    addComponent<ns::ecs::PhysicsComponent>(std::shared_ptr<ns::ecs::PhysicsComponent>(ph));

    addComponent<ns::ecs::InputsComponent>(this);
    inputs()->bind<Player>(sf::Keyboard::Up, &Player::moveUp);
    inputs()->bind<Player>(sf::Keyboard::Down, &Player::moveDown);
    inputs()->bind<Player>(sf::Keyboard::Left, &Player::moveLeft);
    inputs()->bind<Player>(sf::Keyboard::Right, &Player::moveRight);
    inputs()->bind<Player>(ns::Config::Inputs::getButtonKey("B"), &Player::useItem);

    addComponent<ns::ecs::ColliderComponent>(this, new ns::ecs::RectangleCollision(12, 8), sf::Vector2f(0, -4));
}

void Player::useItem() {
    if (game->hud->isOpened()) {
        m_items[game->hud->getItemIndex()].use();
        if (m_items[game->hud->getItemIndex()].getType() != ItemType::PresentButton)
            m_items[game->hud->getItemIndex()] = Item(None);
    }
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

auto Player::getHP() const  -> int{
    return m_hp;
}

auto Player::getFaceDirection() -> sf::Vector2f {
    sf::Vector2f res;
    if (getSprite()->getAnimState() == "walk_side") {
        if (getSprite()->getDrawable().getScale().x == -1)
            res.x = -1;
        else
            res.x = 1;
    }
    else if (getSprite()->getAnimState() == "walk_up")
        res.y = -1;
    else
        res.y = 1;
    return res;

}

void Player::addItem(ItemType item_type) {
    for (auto& m_item : m_items) {
        if (m_item.getType() == None) {
            m_item = Item(item_type);
            break;
        }
    }
}

auto Player::getItems() -> std::array<Item, 3>& {
    return m_items;
}

auto Player::getName() -> const std::string& {
    return m_name;
}

bool Player::isInvisible() const {
    return m_invisibility_timer > 0;
}

void Player::setInvisible() {
    m_invisibility_timer = 200;
}

void Player::restoreHp(int amount) {
    m_hp_to_restore += amount;
}

void Player::damage() {
    if (m_hp > 0 && m_blink_timer == 0) {
        m_hp--;
        m_blink_timer = 80;
    }
}

void Player::update() {

    if (m_hp_to_restore > 0 && game->time.getElapsedTime().asMilliseconds()%500 <= 10) {
        m_hp_to_restore--;
        m_hp = std::min(10, ++m_hp);
    }
    if (m_blink_timer > 0) {
        if (m_blink_timer%4 == 0)
            visible = !visible;
        m_blink_timer--;
    }
    else if (m_invisibility_timer > 0) {
            visible = !visible;
        m_invisibility_timer--;
    }

    if (visible)
        graphics<ns::ecs::SpriteComponent>(0)->getDrawable().setColor(sf::Color::White);
    else
        graphics<ns::ecs::SpriteComponent>(0)->getDrawable().setColor(sf::Color::Transparent);

    physics()->setDirection(0, 0);

    if (ns::Transition::list.empty()) {
        inputs()->update<Player>();
        physics()->update();
    }

    collider()->update();

    // resolve collisions
    for (const auto& rect : MapCollisions::all()) {
        resolveCollision(this, rect);
    }

    for (const auto& ent : Enemy::list) {
        if (ent->getName() != "Wasp")
            resolveCollision(ent.get(), ns::FloatRect(this->collider()->getCollision().getShape().getGlobalBounds()));
    }

    auto sprite = graphics<ns::ecs::SpriteComponent>(0);
    // animate player
    if (physics()->getDirection() == sf::Vector2i(0, 0))
        sprite->getAnimPlayer().stop();
    else {
        if (physics()->getDirection().x == 0) {
            if (physics()->getDirection().y == -1)
                sprite->setAnimState("walk_up");
            else
                sprite->setAnimState("walk_down");
        }
        else if (physics()->getDirection().y == 0){
            if (physics()->getDirection().x == -1)
                sprite->setAnimState("walk_side");
            else
                sprite->setAnimState("walk_side");
        }

        if (sprite->getAnimState() == "walk_down" && physics()->getDirection().y == -1)
            sprite->setAnimState("walk_up");
        if (sprite->getAnimState() == "walk_up" && physics()->getDirection().y == 1)
            sprite->setAnimState("walk_down");

        // dirty code, handles bug when move button pressed before textbox closed
        if (!sprite->getAnimPlayer().isPlaying()) {
            if (physics()->getDirection().y == -1)
                sprite->setAnimState("walk_up");
            else if (physics()->getDirection().y == 1)
                sprite->setAnimState("walk_down");
            else if (physics()->getDirection().x == 1)
                sprite->setAnimState("walk_side");
            else {
                sprite->setAnimState("walk_side");
                sprite->getDrawable().setScale(-1, 1);
            }
        }

        if (sprite->getAnimState() == "walk_side") {
            sprite->getDrawable().setScale(physics()->getDirection().x, 1);
        }
    }

    for (const auto& graphic_comp: graphics()) {
        graphic_comp->update();
    }
}

auto Player::getSprite() -> ns::ecs::SpriteComponent* {
    return graphics<ns::ecs::SpriteComponent>(0);
}
