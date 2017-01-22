#include "Decoration.h"

#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

#include "Singletons.h"

namespace bi {
  static constexpr float SPRITE_SIZE = 256.0f;
  static constexpr float LIMIT_VIEW = 250.0f;

  Decoration::Decoration(const gf::Vector2f postion, const float sizeRendered, float rotate, const gf::Path texturePath)
  : gf::Entity(5)
  , m_texture(&(gResourceManager().getTexture(texturePath)))
  , m_position(postion)
  , m_SIZE_RENDERED(sizeRendered)
  , m_rotation(rotate) {
    m_texture->setSmooth(true);
  }

  void Decoration::update(float dt) {
  }

  void Decoration::render(gf::RenderTarget& target) {
    gf::Sprite sprite;
    sprite.setTexture(*m_texture);
    sprite.setPosition(m_position);
    sprite.setScale(m_SIZE_RENDERED / SPRITE_SIZE);
    sprite.setAnchor(gf::Anchor::Center);
    sprite.setRotation(m_rotation);

    target.draw(sprite);
  }
}
