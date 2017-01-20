#include "Hero.h"

#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

namespace bi {
  void Hero::update(float dt) {
    //
  }

  void Hero::render(gf::RenderTarget& target) {
    gf::CircleShape sprite(10.0f, 3);
    sprite.setColor(gf::Color::Red);
    sprite.setPosition({ 0.0f, 0.0f });

    target.draw(sprite);
  }
}
