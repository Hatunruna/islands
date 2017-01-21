#include "Compass.h"

#include <gf/Curves.h>
#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace bi {
  static constexpr float NORM_COMPASS = 25.0f;
  Compass::Compass()
  : gf::Entity(0)
  , m_angle(0.0f) {
    // Event?
  }

  void Compass::update(float dt) {
  }

  void Compass::render(gf::RenderTarget& target) {
    // Compute debug arrow
    gf::Vector2f center(gWinGeometry().getXCentered(0.0f), gWinGeometry().getYCentered(0.0f));
    gf::Vector2f direction = gf::unit(m_angle) * NORM_COMPASS + center;

    gf::Line sprite(center, direction);
    sprite.setColor(gf::Color::Red);

    target.draw(sprite);
  }
}
