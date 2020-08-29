// Created by Modar Nasser on 29/08/2020.

#include "TextBox.hpp"

using Inputs = ns::Config::Inputs;

TextBox::TextBox(const std::string& string, std::shared_ptr<ns::BitmapFont>& font) :
m_text(string) {
    m_text.setFont(font);
    m_text.setMaxWidth(150);
    m_text.setTypingDelay(2);
    m_text.setMaxLines(3);

    m_bg.setTexture(ns::Res::get().in("fonts").getTexture("frame.png"));

    setPosition(0, (float)ns::Config::Window::view_height-35.f);

}

bool TextBox::opened() const {
    return m_opened;
}

bool TextBox::closed() const {
    return m_bg.getPosition().y > (float)ns::Config::Window::view_height + 10.f;
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
        m_opened = m_bg.getPosition().y <= m_text.getPosition().y -5;
    }
    else {
        m_text.update();
        if (m_closing && !closed()) {
            m_bg.move(0, 2);
        }
    }
}

auto TextBox::getPosition() -> sf::Vector2f {
    return m_text.getPosition() - sf::Vector2f(5, 5);
}

void TextBox::setPosition(const sf::Vector2f& position) {
    m_bg.setPosition(0, (float)ns::Config::Window::view_height);
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
    if (!m_closing && m_opened)
        target.draw(m_text, states);
}
