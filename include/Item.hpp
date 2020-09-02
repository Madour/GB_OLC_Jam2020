// Created by Modar Nasser on 01/09/2020.

#pragma once

class Game;

enum ItemNames {
    None,
    InvisibilityPill,
    Vulnerary,
    PresentButton
};

class Item {
public:
    Item();
    explicit Item(ItemNames item);

    void use();

private:
    ItemNames m_name = None;
    int m_counter = 0;


};



