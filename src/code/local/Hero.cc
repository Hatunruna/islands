#include "Hero.h"

#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>

#include "Messages.h"
#include "Singletons.h"

namespace bi {
  static constexpr float ANGULAR_VELOCITY = 1.0f;
  static constexpr float VELOCITY = 100.0f;
  static constexpr float SPRITE_SIZE = 256.0f;
  static constexpr float HERO_SIZE = 64.0f;
  static constexpr float STEP_TIME = 0.25f;
  static constexpr float STEP_ANGLE = 10.0f * gf::Pi / 180.0f; // rad

  Hero::Hero(const gf::Vector2f postion)
  : gf::Entity(10)
  , m_turn(Turn::NONE)
  , m_move(Move::NONE)
  , m_position(postion)
  , m_angle(0.0f)
  , m_texture(gResourceManager().getTexture("tricorn.png"))
  , m_timeElapsed(0.0f)
  , m_alternateStep(true)
  , m_isOnIsland(true)
  , m_isFrozen(false) {
    m_texture.setSmooth(true);

    // Register message
    gMessageManager().registerHandler<StartScan>(&Hero::onStartScan, this);
    gMessageManager().registerHandler<StopScan>(&Hero::onStopScan, this);
  }

  void Hero::moveForward() {
    m_move = Move::FORWARD;
  }

  void Hero::moveBackward() {
    m_move = Move::BACKWARD;
  }

  void Hero::moveStop() {
    m_move = Move::NONE;
  }

  void Hero::turnLeft() {
    m_turn = Turn::LEFT;
  }

  void Hero::turnRight() {
    m_turn = Turn::RIGHT;
  }

  void Hero::turnNone() {
    m_turn = Turn::NONE;
  }

  void Hero::sendInitialPosition() const {
    // Send the position message
    HeroPosition message;
    message.position = m_position;
    gMessageManager().sendMessage(&message);
  }

  bool Hero::isOnIsland() const {
    return m_isOnIsland;
  }

  bool Hero::scanAvailable() const {
    return m_isOnIsland && !m_isFrozen;
  }

  gf::MessageStatus Hero::onStartScan(gf::Id id, gf::Message *msg) {
    assert(id == StartScan::type);
    // auto startScan = static_cast<StartScan*>(msg);

    m_isFrozen = true;

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Hero::onStopScan(gf::Id id, gf::Message *msg) {
    assert(id == StopScan::type);
    // auto startScan = static_cast<StartScan*>(msg);

    m_isFrozen = false;

    return gf::MessageStatus::Keep;
  }

  void Hero::update(float dt) {
    m_timeElapsed += dt;

    // If not forzen
    if (!m_isFrozen) {
      // Set the new angle
      switch (m_turn) {
      case Turn::RIGHT:
        m_angle += ANGULAR_VELOCITY * dt;
        break;
      case Turn::LEFT:
        m_angle -= ANGULAR_VELOCITY * dt;
        break;
      case Turn::NONE:
        // Nothing
        break;
      }

      // Manage the step "animation"
      if (m_timeElapsed > STEP_TIME) {
        m_timeElapsed -= STEP_TIME;
        m_alternateStep = !m_alternateStep;
      }

      // Set the velocity
      float velocity = 0.0f;
      switch (m_move) {
      case Move::FORWARD:
        velocity = VELOCITY * dt;
        break;

      case Move::BACKWARD:
        velocity = -VELOCITY * dt;
        break;

      case Move::NONE:
        // Nothing
        break;
      }

      // Compute the new position
      m_position += gf::unit(m_angle) * velocity;
    }

    // Send the position message
    HeroPosition message;
    message.position = m_position;
    gMessageManager().sendMessage(&message);

    m_isOnIsland = message.isOnIsland;
  }

  void Hero::render(gf::RenderTarget& target) {
    gf::Sprite sprite;

    // Render the step
    float angleRendered = m_angle;
    if (!m_isFrozen && m_alternateStep && (m_move == Move::FORWARD || m_move == Move::BACKWARD)) {
      angleRendered += STEP_ANGLE;
    }
    else if (!m_isFrozen && !m_alternateStep && (m_move == Move::FORWARD || m_move == Move::BACKWARD)) {
      angleRendered -= STEP_ANGLE;
    }
    sprite.setRotation(angleRendered - gf::Pi2); // Pi/2 to align the hero front face

    sprite.setTexture(m_texture);
    sprite.setPosition(m_position);
    sprite.setScale(HERO_SIZE / SPRITE_SIZE);
    sprite.setAnchor(gf::Anchor::Center);

    target.draw(sprite);
  }
}
