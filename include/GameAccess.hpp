// Created by Modar Nasser on 02/09/2020.

#pragma once

class Game;

class GameAccess {
    friend Game;
protected:
    static Game* game;
};



