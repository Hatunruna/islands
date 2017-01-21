#include "Compass.h"

#include <gf/Curves.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace bi {
  static constexpr float NORM_COMPASS = 25.0f;
  static constexpr float SCAN_COOLDOWN = 6.0f;
  static constexpr float COMPASS_SIZE = 80.0f;
  static constexpr float POINTER_SIZE = 80.0f;
  static constexpr float SPRITE_SIZE = 256.0f;

  Compass::Compass()
  : gf::Entity(0)
  , m_angle(0.0f)
  , m_timeElapsed(0.0f)
  , m_displayed(false)
  , m_compassTexture(gResourceManager().getTexture("compass.png"))
  , m_pointerTexture(gResourceManager().getTexture("pointer.png")) {
    // Register message
    gMessageManager().registerHandler<StartScan>(&Compass::onStartScan, this);
    gMessageManager().registerHandler<NearestTreasure>(&Compass::onNearestTreasure, this);
  }

  gf::MessageStatus Compass::onStartScan(gf::Id id, gf::Message *msg) {
    assert(id == StartScan::type);
    // auto startScan = static_cast<StartScan*>(msg);

    m_displayed = true;

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Compass::onNearestTreasure(gf::Id id, gf::Message *msg) {
    assert(id == NearestTreasure::type);
    auto nearestTreasure = static_cast<NearestTreasure*>(msg);

    m_angle = gf::angle(nearestTreasure->position);

    return gf::MessageStatus::Keep;
  }

  void Compass::update(float dt) {
    if (m_displayed) {
      m_timeElapsed += dt;

      if (m_timeElapsed > SCAN_COOLDOWN) {
        m_timeElapsed = 0.0f;
        m_displayed = false;
        StopScan message;
        gMessageManager().sendMessage(&message);
      }
    }
  }

  void Compass::render(gf::RenderTarget& target) {
    if (m_displayed) {
      gf::Vector2f center(gWinGeometry().getXCentered(0.0f), gWinGeometry().getYCentered(0.0f));

      // Draw the background
      gf::Sprite compass;
      compass.setTexture(m_compassTexture);
      compass.setScale(COMPASS_SIZE / SPRITE_SIZE);
      compass.setPosition(center);
      compass.setAnchor(gf::Anchor::Center);
      target.draw(compass);

      // Draw the pointer
      gf::Sprite pointer;
      pointer.setTexture(m_pointerTexture);
      pointer.setScale(POINTER_SIZE / SPRITE_SIZE);
      pointer.setPosition(center);
      pointer.setRotation(m_angle);
      pointer.setAnchor(gf::Anchor::Center);
      target.draw(pointer);

      gf::Vector2f centerArrow(gWinGeometry().getXCentered(0.0f), gWinGeometry().getYCentered(0.0f));
      gf::Vector2f direction = gf::unit(m_angle) * NORM_COMPASS + centerArrow;
      gf::Line sprite(centerArrow, direction);
      sprite.setColor(gf::Color::Red);

      target.draw(sprite);
    }
  }
}
