// Created by Modar Nasser on 29/08/2020.

#pragma once
#include "../NasNas.h"
#include "GameAccess.hpp"

class TextBox : public ns::Drawable, GameAccess {
public:
    TextBox(const std::string& string, std::shared_ptr<ns::BitmapFont>& font, const std::string& label = "");

    auto getPosition() -> sf::Vector2f override;
    void setPosition(const sf::Vector2f& position);
    void setPosition(float x, float y);
    auto getGlobalBounds() -> ns::FloatRect override;

    bool opened() const;
    bool closed() const;

    void onEvent(const sf::Event& event);
    void update();

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool m_opened = false;
    bool m_closing = false;

    ns::ui::TypedText m_text;
    ns::BitmapText m_label;
    sf::Sprite m_bg;
    sf::Sprite m_bg_label;

};



