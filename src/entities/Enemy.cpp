// Created by Modar Nasser on 02/09/2020.

#include "entities/Enemy.hpp"
#include "entities/Wasp.hpp"
#include "entities/BigMummy.hpp"
#include "Game.hpp"

std::vector<std::shared_ptr<Enemy>> Enemy::list;

std::unordered_map<std::string, std::function<Enemy*()>> Enemy::enemy_factory;

auto Enemy::createFromName(const std::string& name) -> Enemy* {
    if (Enemy::enemy_factory.empty()) {
        enemy_factory = {
            {"Wasp", [](){ return new Wasp(); }},
            {"BigMummy", [](){ return new BigMummy(); }},
            {"SmallMummy", [](){ return new BigMummy(); }},
        };
    }
    if (Enemy::enemy_factory.count(name) > 0)
        return Enemy::enemy_factory.at(name)();
    else {
        ns_LOG("Error : Enemy ", name, " does not exist in database. ");
        return nullptr;
    }
}

Enemy::Enemy(const std::string& name) : ns::BaseEntity(name) {
    list.push_back(std::shared_ptr<Enemy>(this));
}

bool Enemy::targetLocked() const {
    return m_target_locked;
}

auto Enemy::getDistanceToTarget() const -> const sf::Vector2f& {
    return m_distance_to_target;
}
