#include "Compass.h"

#include <gf/Curves.h>
#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace bi {
  static constexpr float NORM_COMPASS = 25.0f;
  static constexpr float SCAN_COOLDOWN = 6.0f;

  Compass::Compass()
  : gf::Entity(0)
  , m_angle(0.0f)
  , m_timeElapsed(0.0f)
  , m_displayed(false) {
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
      // Compute debug arrow
      gf::Vector2f center(gWinGeometry().getXCentered(0.0f), gWinGeometry().getYCentered(0.0f));
      gf::Vector2f direction = gf::unit(m_angle) * NORM_COMPASS + center;

      gf::Line sprite(center, direction);
      sprite.setColor(gf::Color::Red);

      target.draw(sprite);
    }
  }
}
