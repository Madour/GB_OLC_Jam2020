// Created by Modar Nasser on 02/09/2020.

#pragma once

#include "../NasNas.h"
#include "GameAccess.hpp"

class Enemy : public ns::BaseEntity, GameAccess {
public:
    static std::vector<Enemy*> list;

    Enemy();

    void update() override;

protected:
    bool m_target_locked = false;
};



