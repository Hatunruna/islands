#include "Treasure.h"

#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

#include "Singletons.h"

namespace bi {
  static constexpr float SPRITE_SIZE = 256.0f;

  Treasure::Treasure(const gf::Vector2f postion, const float sizeRendered, const gf::Path texturePath)
  : gf::Entity(10)
  , m_texture(gResourceManager().getTexture(texturePath))
  , m_position(postion)
  , m_SIZE_RENDERED(sizeRendered) {
    m_texture.setSmooth(true);
  }

  gf::Vector2f Treasure::getPosition() const {
    return m_position;
  }

  void Treasure::update(float dt) {
    // Nothing
  }

  void Treasure::render(gf::RenderTarget& target) {
    gf::Sprite sprite;
    sprite.setTexture(m_texture);
    sprite.setPosition(m_position);
    sprite.setScale(m_SIZE_RENDERED / SPRITE_SIZE);
    sprite.setAnchor(gf::Anchor::Center);

    target.draw(sprite);
  }
}
