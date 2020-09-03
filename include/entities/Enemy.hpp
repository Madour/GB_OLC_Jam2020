// Created by Modar Nasser on 02/09/2020.

#pragma once

#include "../../NasNas.h"
#include "GameAccess.hpp"

class Enemy : public ns::BaseEntity, protected GameAccess {
public:
    static std::vector<std::shared_ptr<Enemy>> list;

    static auto createFromName(const std::string& name) -> Enemy*;

    bool targetLocked() const;

    auto getDistanceToTarget() const -> const sf::Vector2f&;


protected:
    explicit Enemy(const std::string& name);

    sf::Vector2f m_distance_to_target;
    sf::Vector2f m_target_pos;
    bool m_target_locked = false;

private:

    static std::unordered_map<std::string, std::function<Enemy*()>> enemy_factory;
};



