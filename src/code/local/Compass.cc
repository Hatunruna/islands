#include "Compass.h"

#include <gf/Color.h>
#include <gf/Curves.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace bi {
  static constexpr float NORM_COMPASS = 25.0f;
  static constexpr float SCAN_COOLDOWN = 6.0f;
  static constexpr float COMPASS_SIZE = 80.0f;
  static constexpr float POINTER_SIZE = 80.0f;
  static constexpr float RANGE_ANGLE = 180.0f * gf::Pi / 180;
  static constexpr float SPRITE_SIZE = 256.0f;
  static constexpr unsigned NUMBER_FRAME_TO_NEXT_ANGLE = 20;

  static constexpr float TIME_BETWEEN_RADARS = 1.5f;
  static constexpr float RADAR_RADIUS_INCREASE = 100.0f;
  static constexpr float RADAR_LIFETIME = 5.0f;

  Compass::Compass()
  : gf::Entity(0)
  , m_perfectAngleTarget(0.0f)
  , m_estimatedAngleTarget(0.0f)
  , m_currentAngleTarget(0.0f)
  , m_stepAngle(0.0f)
  , m_currentStep(0)
  , m_timeElapsed(0.0f)
  , m_displayed(false)
  , m_compassTexture(gResourceManager().getTexture("compass.png"))
  , m_pointerTexture(gResourceManager().getTexture("pointer.png"))
  , m_timeUntilNextRadar(0)
  {
    // Register message
    gMessageManager().registerHandler<StartScan>(&Compass::onStartScan, this);
    gMessageManager().registerHandler<NearestTreasure>(&Compass::onNearestTreasure, this);
  }

  gf::MessageStatus Compass::onStartScan(gf::Id id, gf::Message *msg) {
    assert(id == StartScan::type);
    // auto startScan = static_cast<StartScan*>(msg);

    m_displayed = true;
    m_radars.clear();
    m_timeUntilNextRadar = 0;

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Compass::onNearestTreasure(gf::Id id, gf::Message *msg) {
    assert(id == NearestTreasure::type);
    auto nearestTreasure = static_cast<NearestTreasure*>(msg);

    // Get the real target
    m_perfectAngleTarget = gf::angle(nearestTreasure->position);

    // Generate the first fake target
    m_estimatedAngleTarget = gRandom().computeUniformFloat(m_perfectAngleTarget - RANGE_ANGLE, m_perfectAngleTarget + RANGE_ANGLE);

    // Generate the current target
    m_currentAngleTarget = gRandom().computeUniformFloat(m_perfectAngleTarget - RANGE_ANGLE, m_perfectAngleTarget + RANGE_ANGLE);

    // Compute the step between fake target and current target
    m_stepAngle = (m_estimatedAngleTarget - m_currentAngleTarget) / NUMBER_FRAME_TO_NEXT_ANGLE;
    m_currentStep = 0;

    return gf::MessageStatus::Keep;
  }

  void Compass::update(float dt) {
    if (m_displayed) {
      m_timeElapsed += dt;

      // If the fake target is not reached
      if (m_currentStep < NUMBER_FRAME_TO_NEXT_ANGLE) {
        m_currentStep++;
        m_currentAngleTarget += m_stepAngle;
      }
      // If the new kake targe is needed
      else {
        float newLimit = RANGE_ANGLE / (m_timeElapsed * 4.0f);
        m_currentAngleTarget = m_estimatedAngleTarget;
        m_estimatedAngleTarget = gRandom().computeUniformFloat(m_perfectAngleTarget - newLimit, m_perfectAngleTarget + newLimit);
        m_stepAngle = (m_estimatedAngleTarget - m_currentAngleTarget) / NUMBER_FRAME_TO_NEXT_ANGLE;
        m_currentStep = 0;
      }

      if (m_timeElapsed > SCAN_COOLDOWN) {
        m_timeElapsed = 0.0f;
        m_displayed = false;
        StopScan message;
        gMessageManager().sendMessage(&message);
      }

      // radars

      while (!m_radars.empty() && m_radars.front().lifetime < 0) {
        m_radars.pop_front();
      }

      m_timeUntilNextRadar -= dt;

      if (m_timeUntilNextRadar < 0) {
        m_timeUntilNextRadar += TIME_BETWEEN_RADARS;

        Radar radar;
        radar.radius = COMPASS_SIZE;
        radar.lifetime = RADAR_LIFETIME;

        m_radars.push_back(radar);
      }

      for (auto& radar : m_radars) {
        radar.radius += RADAR_RADIUS_INCREASE * dt;
        radar.lifetime -= dt;
      }
    }
  }

  void Compass::render(gf::RenderTarget& target) {
    if (m_displayed) {
      gf::Vector2f center(gWinGeometry().getXCentered(0.0f), gWinGeometry().getYCentered(0.0f));
      float displaySize = 15 * gWinGeometry().getYFromBottom(0.0f) / 100.0f;

      gf::Color4f radarColor = gf::Color::fromRgba32(0x4C, 0x3C, 0x34);

      gf::CircleShape circle;
      circle.setColor(gf::Color::Transparent);
      circle.setOutlineThickness(5.0f);
      circle.setPosition(center);
      circle.setScale(displaySize / SPRITE_SIZE);

      // Draw the radar
      for (const auto& radar : m_radars) {
        radarColor.a = radar.lifetime / RADAR_LIFETIME;
        circle.setOutlineColor(radarColor);
        circle.setRadius(radar.radius);
        circle.setAnchor(gf::Anchor::Center);
        target.draw(circle);
      }

      // Draw the background
      gf::Sprite compass;
      compass.setTexture(m_compassTexture);
      compass.setScale(displaySize / SPRITE_SIZE);
      compass.setPosition(center);
      compass.setAnchor(gf::Anchor::Center);
      target.draw(compass);

      // Draw the pointer
      gf::Sprite pointer;
      pointer.setTexture(m_pointerTexture);
      pointer.setScale(displaySize / SPRITE_SIZE);
      pointer.setPosition(center);
      pointer.setRotation(m_currentAngleTarget);
      pointer.setAnchor(gf::Anchor::Center);
      target.draw(pointer);
    }
  }
}
