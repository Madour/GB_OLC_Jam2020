// Created by Modar Nasser on 29/08/2020.

#include "TextBox.hpp"
#include "Palette.hpp"
#include "Game.hpp"

using Inputs = ns::Config::Inputs;


TextBox::TextBox(const std::string& string, std::shared_ptr<ns::BitmapFont>& font, const std::string& label) :
m_text(string),
m_label(label) {
    m_text.setFont(font);
    m_text.setColor(Palette::Base[0]);
    m_text.setMaxWidth(150);
    m_text.setTypingDelay(2);
    m_text.setMaxLines(3);

    m_label.setFont(game->fonts.at("italic"));
    m_label.setColor(Palette::Base[0]);

    m_bg.setTexture(ns::Res::get().in("fonts").getTexture("frame.png"));
    m_bg.setTextureRect({0, 13, 160, 35});

    m_bg_label.setTexture(ns::Res::get().in("fonts").getTexture("frame.png"));
    m_bg_label.setTextureRect({0, 0, 160, 13});

    setPosition(0, (float)ns::Config::Window::view_height+12.f);
    m_text.setPosition(5, (float)ns::Config::Window::view_height-31.f);

}

bool TextBox::opened() const {
    return m_opened;
}

bool TextBox::closed() const {
    return m_bg.getPosition().y > ((float)ns::Config::Window::view_height + 15.f);
}

void TextBox::onEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == Inputs::getButtonKey("A")
        || event.key.code == Inputs::getButtonKey("B")
        || event.key.code == Inputs::getButtonKey("select") ) {
            if (m_text.hasEnded())
                m_closing = true;
            else if (m_text.isWaiting()) {
                m_text.nextPage();
                m_text.setTypingDelay(3);
            }
            else
                m_text.setTypingDelay(-1);

        }
    }
}

void TextBox::update() {
    if (!m_opened) {
        m_bg.move(0, -2);
        m_bg_label.move(0, -2);
        m_label.move(0, -2);
        m_opened = m_bg.getPosition().y <= m_text.getPosition().y-5;
    }
    else {
        if (!m_text.isWaiting() && !m_text.hasEnded() && game->getTick()%6 == 0)
            game->playSound("bip");
        m_text.update();
        if (m_closing && !closed()) {
            m_bg.move(0, 2);
            m_text.move(0, 2);
            m_bg_label.move(0, 2);
            m_label.move(0, 2);
        }
    }
}

auto TextBox::getPosition() -> sf::Vector2f {
    return m_text.getPosition() - sf::Vector2f(5, 5);
}

void TextBox::setPosition(const sf::Vector2f& position) {
    m_bg.setPosition(position.x, position.y);
    m_bg_label.setPosition(position.x, position.y-12);
    m_label.setPosition(position.x+12, position.y-8);
    m_text.setPosition(position + sf::Vector2f(5, 5));
}

void TextBox::setPosition(float x, float y) {
    setPosition({x, y});
}

auto TextBox::getGlobalBounds() -> ns::FloatRect {
    return ns::FloatRect(m_bg.getGlobalBounds());
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_bg, states);
    if (!m_label.getString().empty()) {
        target.draw(m_bg_label);
        target.draw(m_label, states);
    }
    if (m_opened)
        target.draw(m_text, states);
}
