/**
* Created by Modar Nasser on 10/06/2020.
**/


#include "NasNas/ecs/SpriteComponent.hpp"
#include "NasNas/ecs/BaseEntity.hpp"
#include <cmath>

using namespace ns;
using namespace ns::ecs;

SpriteComponent::SpriteComponent(BaseEntity* entity, Spritesheet* spritesheet, const std::string& anim_state) :
GraphicsComponent(entity)
{
    setSpritesheet(spritesheet);
    m_anim_player = AnimPlayer();
    setAnimState(anim_state);
}

SpriteComponent::SpriteComponent(BaseEntity* entity, Spritesheet* spritesheet, const std::string& anim_state, const sf::Vector2f& pos_offset) :
SpriteComponent(entity, spritesheet, anim_state)
{
    m_pos_offset = pos_offset;
}

void SpriteComponent::setSpritesheet(Spritesheet* spritesheet) {
    m_spritesheet = spritesheet;
    m_drawable = sf::Sprite(*spritesheet->texture);
}

auto SpriteComponent::getAnimState() -> const std::string& {
    return m_anim_player.getAnim()->getName();
}

void SpriteComponent::setAnimState(const std::string& anim_state) {
    if (m_spritesheet) {
        m_anim_player.resume();
        if (!m_anim_player.getAnim() || anim_state != m_anim_player.getAnim()->getName()) {
            try {
                m_anim_player.play(m_spritesheet->getAnim(anim_state));
                m_drawable.setTextureRect(m_anim_player.getActiveFrame().rectangle);
                m_drawable.setOrigin((float)m_anim_player.getActiveFrame().origin.x, (float)m_anim_player.getActiveFrame().origin.y);
            }
            catch (std::invalid_argument& exception) {
                std::cout << exception.what() << std::endl;
                exit(-1);
            }
        }
    }
}

auto SpriteComponent::getAnimPlayer() -> AnimPlayer& {
    return m_anim_player;
}

auto SpriteComponent::getDrawable() -> sf::Sprite& {
    return m_drawable;
}

auto SpriteComponent::getGlobalBounds() -> ns::FloatRect {
    return ns::FloatRect(m_drawable.getGlobalBounds());
}

void SpriteComponent::update() {
    m_anim_player.update();
    m_drawable.setTextureRect(m_anim_player.getActiveFrame().rectangle);
    m_drawable.setOrigin((float)m_anim_player.getActiveFrame().origin.x, (float)m_anim_player.getActiveFrame().origin.y);
    auto new_pos = sf::Vector2f(std::round(m_entity->getPosition().x), std::round(m_entity->getPosition().y)) + m_pos_offset;
    m_drawable.setPosition(new_pos);
}

void SpriteComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_drawable, states);
}
