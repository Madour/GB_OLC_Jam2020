/**
* Created by Modar Nasser on 25/06/2020.
**/


#include "NasNas/core/BitmapText.hpp"
#include <cstring>

using namespace ns;

BitmapGlyph::BitmapGlyph(const ns::IntRect& texture_rect, char character, int spacing) :
texture_rect(texture_rect),
character(character),
advance(spacing)
{}


BitmapFont::BitmapFont(const sf::Texture& texture, const sf::Vector2u& glyph_size) :
BitmapFont(texture, glyph_size, " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~", {}, glyph_size)
{}

BitmapFont::BitmapFont(const sf::Texture& texture, const sf::Vector2u& glyph_size, const std::string& chars_map) :
BitmapFont(texture, glyph_size, chars_map, {}, glyph_size)
{}

BitmapFont::BitmapFont(const sf::Texture& texture, const sf::Vector2u& glyph_size, const std::unordered_map<std::string, int>& spacings_map) :
BitmapFont(texture, glyph_size, " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~", spacings_map, glyph_size)
{}

BitmapFont::BitmapFont(const sf::Texture& texture, const sf::Vector2u& glyph_size, const std::string& chars_map, const std::unordered_map<std::string, int>& spacings_map, sf::Vector2u default_advance) :
m_texture(&texture),
m_glyph_size(glyph_size) {
    m_chars_map = chars_map;
    for (const auto& [string, advance] : spacings_map) {
        for (const auto& letter : string) {
            if (m_advance_map.count(letter) == 0)
                m_advance_map[letter] = advance;
        }
    }

    unsigned int i = 0;
    for (int y = 0; y < (int)m_texture->getSize().y; y += m_glyph_size.y) {
        for (int x = 0; x < (int)m_texture->getSize().x; x += m_glyph_size.x) {
            if (i < m_chars_map.size()) {
                char character = m_chars_map[i];
                if (m_glyphs.count(character) == 0) {
                    int spacing = default_advance.x;
                    if (m_advance_map.count(character) > 0)
                        spacing = m_advance_map[character];
                    m_glyphs[character] = new BitmapGlyph({{x, y}, {(int)m_glyph_size.x, (int)m_glyph_size.y}}, character, spacing);
                }
            }
            i += 1;
        }
    }
}

BitmapFont::~BitmapFont() {
    for (const auto& [name, glyph] : m_glyphs) {
        delete(glyph);
    }
}

auto BitmapFont::getGlyphSize() -> const sf::Vector2u& {
    return m_glyph_size;
}

auto BitmapFont::getTexture() -> const sf::Texture* {
    return m_texture;
}

auto BitmapFont::getGlyph(char character) -> const BitmapGlyph& {
    if (m_glyphs.count(character) > 0) {
        return *m_glyphs[character];
    }
    std::cout << "Selected BitmapFont does not have glyph for character «" << character << "» " << std::endl;
    exit(-1);
}

auto BitmapFont::computeStringSize(const std::string& string) -> sf::Vector2i {
    int h = getGlyphSize().y;
    int w = 0, max_width = 0;
    for (const auto character : string) {
        if (character == '\n') {
            h += getGlyphSize().y;
            max_width = std::max(w, max_width);
            w = 0;
            continue;
        }
        w += getGlyph(character).advance;
    }
    max_width = std::max(w, max_width);
    return {max_width, h};
}


BitmapText::BitmapText(const std::string& text) {
    m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
    setString(text);
}

auto BitmapText::getString() const -> const std::string& {
    return m_string;
}

void BitmapText::setString(const std::string &string) {
    m_string = string;
    processString();
    updateVertices();
}

auto BitmapText::getFont() -> std::shared_ptr<BitmapFont>& {
    return m_font;
}

void BitmapText::setFont(const std::shared_ptr<BitmapFont>& font) {
    m_font = font;
    processString();
    updateVertices();
}

void BitmapText::setColor(const sf::Color &color) {
    m_color = color;
    updateVertices();
}

auto BitmapText::getColor() -> const sf::Color& {
    return m_color;
}

auto BitmapText::getPosition() -> sf::Vector2f {
    return m_transformable.getPosition();
}

void BitmapText::setPosition(const sf::Vector2f& position) {
    m_transformable.setPosition(position);
}

void BitmapText::setPosition(float x, float y) {
    m_transformable.setPosition(x, y);
}

auto BitmapText::getGlobalBounds() -> ns::FloatRect {
    return ns::FloatRect(m_transformable.getPosition(), getSize());
}

void BitmapText::setMaxWidth(int max_width) {
    m_max_width = max_width;
    processString();
    updateVertices();
}

auto BitmapText::getWidth() const -> int {
    return m_width;
}

auto BitmapText::getHeight() const -> int {
    return m_height;
}

auto BitmapText::getSize() const -> sf::Vector2f {
    return sf::Vector2f((float)getWidth(), (float)getHeight());
}

void BitmapText::move(float offsetx, float offsety) {
    m_transformable.move(offsetx, offsety);
}

auto BitmapText::getProcessedString() -> const std::string& {
    return m_processed_string;
}

void BitmapText::processString() {
    m_processed_string = m_string;
    if (m_max_width > 0 && m_font != nullptr) {
        // splitting string into words
        std::vector<std::string> words;
        auto str = m_string;
        auto* word = std::strtok(str.data(), " ");
        while (word != nullptr) {
            words.emplace_back(word);
            word = std::strtok(nullptr, " ");
        }
        // inserting new line character when the width exceeds the max width
        int current_width = 0;
        for (auto& w : words) {
            if (w[0] == '\n' || w[w.size()-1] == '\n') current_width = 0;
            if (current_width + getFont()->computeStringSize(w).x > m_max_width) {
                current_width = 0;
                if (w[0] != '\n')
                    w = std::string("\n").append(w);
            }
            current_width += getFont()->computeStringSize(w+" ").x;
        }
        // joining the words into the final processed string
        m_processed_string.clear();
        for (int i = 0; i < words.size(); ++i) {
            if ((i+1 < words.size() && words[i+1][0] == '\n') || i+1 == words.size()) {
                m_processed_string += words[i];
                continue;
            }
            m_processed_string += words[i] + " ";
        }
    }
}

void BitmapText::updateVertices() {
    m_vertices.clear();
    if (m_font != nullptr) {
        float x = 0, y = 0;
        int w = 0, h = m_font->getGlyphSize().y;
        int max_w = 0;
        for(const auto& character : m_processed_string) {
            if (character == '\n') {
                x = 0;
                y += (float)m_font->getGlyphSize().y+1.f;
                max_w = std::max(max_w, w);
                w = 0;
                h += m_font->getGlyphSize().y+1u;
                continue;
            }
            auto glyph = m_font->getGlyph(character);
            auto glyph_size = (sf::Vector2f)m_font->getGlyphSize();
            auto vertex_tl = sf::Vertex({x, y}, m_color, (sf::Vector2f)glyph.texture_rect.topleft());
            auto vertex_tr = sf::Vertex({x + glyph_size.x, y}, m_color, (sf::Vector2f)glyph.texture_rect.topright());
            auto vertex_br = sf::Vertex({x + glyph_size.x, y + glyph_size.y}, m_color, (sf::Vector2f)glyph.texture_rect.bottomright());
            auto vertex_bl = sf::Vertex({x, y + glyph_size.y}, m_color, (sf::Vector2f)glyph.texture_rect.bottomleft());
            m_vertices.append(vertex_tl);
            m_vertices.append(vertex_tr);
            m_vertices.append(vertex_br);
            m_vertices.append(vertex_bl);
            x += (float)glyph.advance;
            w += glyph.advance;
        }
        m_width = std::max(max_w, w);
        m_height = h;
    }
}

void BitmapText::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (m_font != nullptr) {
        states.texture = m_font->getTexture();
        states.transform *= m_transformable.getTransform();
        target.draw(m_vertices, states);
    }
}
