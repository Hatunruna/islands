#include "Treasure.h"

#include <gf/Math.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace bi {
  static constexpr float SPRITE_SIZE = 256.0f;
  static constexpr float LIMIT_VIEW = 250.0f;

  Treasure::Treasure(const gf::Vector2f postion, const float sizeRendered, const gf::Path texturePath)
  : gf::Entity(5)
  , m_texture(&(gResourceManager().getTexture(texturePath)))
  , m_position(postion)
  , m_SIZE_RENDERED(sizeRendered)
  , m_alphaLevel(0.0f)
  , m_heroPosition({ 0.0f, 0.0f })
  , m_isFound(false) {
    m_texture->setSmooth(true);
  }

  gf::Vector2f Treasure::getPosition() const {
    return m_position;
  }

  void Treasure::setHeroPosition(const gf::Vector2f heroPosition) {
    m_heroPosition = heroPosition;
  }

  bool Treasure::isFound() const {
    return m_isFound;
  }

  void Treasure::found() {
    m_isFound = true;
  }

  void Treasure::update(float dt) {
    float distance = gf::euclideanDistance(m_position, m_heroPosition);
    m_alphaLevel = (LIMIT_VIEW - distance) / (LIMIT_VIEW - 50.0f);
    gf::clamp(m_alphaLevel, 0.0f, 1.0f);
  }

  void Treasure::render(gf::RenderTarget& target) {
    gf::Sprite sprite;
    sprite.setTexture(*m_texture);
    sprite.setPosition(m_position);
    sprite.setScale(m_SIZE_RENDERED / SPRITE_SIZE);
    sprite.setColor({ 1.0f, 1.0f, 1.0f, m_alphaLevel });
    sprite.setAnchor(gf::Anchor::Center);

    target.draw(sprite);
  }
}
