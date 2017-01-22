#include "Hero.h"

#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>

#include "Messages.h"
#include "Singletons.h"

namespace bi {
  static constexpr float HERO_ANGULAR_VELOCITY = 1.5f;
  static constexpr float HERO_VELOCITY = 100.0f;

  static constexpr float BOAT_ANGULAR_VELOCITY = 0.5f;
  static constexpr float BOAT_VELOCITY = 200.0f;

  static constexpr float SPRITE_SIZE = 256.0f;
  static constexpr float HERO_SIZE = 64.0f;
  static constexpr float BOAT_SIZE = 128.0f;

  static constexpr float STEP_TIME = 0.25f;
  static constexpr float STEP_ANGLE = 10.0f * gf::Pi / 180.0f; // rad

  Hero::Hero(Steam& steam, const gf::Vector2f postion)
  : gf::Entity(10)
  , m_steam(steam)
  , m_turn(Turn::NONE)
  , m_move(Move::NONE)
  , m_position(postion)
  , m_angle(0.0f)
  , m_hatTexture(gResourceManager().getTexture("tricorn.png"))
  , m_boatTexture(gResourceManager().getTexture("boat.png"))
  , m_timeElapsed(0.0f)
  , m_alternateStep(true)
  , m_isOnIsland(true)
  , m_isFrozen(false)
  , m_boatSound(gResourceManager().getSound("pirate-boat.wav"))
  , m_pathSound(gResourceManager().getSound("pirate-path.wav"))
  , m_isGameOver(false) {
    m_hatTexture.setSmooth(true);
    m_boatTexture.setSmooth(true);

    // Register message
    gMessageManager().registerHandler<StartScan>(&Hero::onStartScan, this);
    gMessageManager().registerHandler<StopScan>(&Hero::onStopScan, this);
    gMessageManager().registerHandler<GameOver>(&Hero::onGameOver, this);

    m_boatSound.setLoop(true);
    m_pathSound.setLoop(true);
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

  gf::MessageStatus Hero::onGameOver(gf::Id id, gf::Message *msg) {
    assert(id == GameOver::type);

    m_boatSound.stop();
    m_pathSound.stop();
    m_isGameOver = true;

    return gf::MessageStatus::Keep;
  }

  void Hero::update(float dt) {
    if (m_isGameOver) {
      return;
    }
    m_timeElapsed += dt;

    // If not frozen
    if (!m_isFrozen) {
      // Set the new angle
      float angularVelocity = m_isOnIsland ? HERO_ANGULAR_VELOCITY : BOAT_ANGULAR_VELOCITY;

      switch (m_turn) {
      case Turn::RIGHT:
        m_angle += angularVelocity * dt;
        break;
      case Turn::LEFT:
        m_angle -= angularVelocity * dt;
        break;
      case Turn::NONE:
        // Nothing
        break;
      }

      // Manage the step "animation"
      while (m_timeElapsed > STEP_TIME) {
        m_timeElapsed -= STEP_TIME;
        m_alternateStep = !m_alternateStep;
      }

      // Set the velocity
      float velocity = m_isOnIsland ? HERO_VELOCITY : BOAT_VELOCITY;
      float distance = 0.0f;

      switch (m_move) {
      case Move::FORWARD:
        distance = velocity * dt;
        break;

      case Move::BACKWARD:
        distance = -velocity * dt;
        break;

      case Move::NONE:
        m_pathSound.stop();
        // Nothing
        break;
      }

      // Compute the new position
      m_position += gf::unit(m_angle) * distance;
    }

    if (!m_isOnIsland) {
      m_steam.run();
      if (m_boatSound.getStatus() != sf::SoundSource::Playing) {
        m_boatSound.play();
      }
      m_pathSound.stop();
    } else {
      m_steam.stop();
      if (m_move != Move::NONE) {
        if (m_pathSound.getStatus() != sf::SoundSource::Playing) {
          m_pathSound.play();
        }
      }
      m_boatSound.stop();
    }

    // Send the position message
    HeroPosition message;
    message.position = m_position;
    message.angle = m_angle;
    gMessageManager().sendMessage(&message);

    m_isOnIsland = message.isOnIsland;
    m_position = message.position;
  }

  void Hero::render(gf::RenderTarget& target) {
    gf::Sprite sprite;

    if (m_isOnIsland) {
      // Render the step
      float angleRendered = m_angle;

      if (!m_isFrozen && m_move != Move::NONE) {
        if (m_alternateStep) {
          angleRendered += STEP_ANGLE;
        } else {
          angleRendered -= STEP_ANGLE;
        }
      }

      sprite.setTexture(m_hatTexture);

      sprite.setScale(HERO_SIZE / SPRITE_SIZE);
      sprite.setRotation(angleRendered - gf::Pi2); // Pi/2 to align the hero front face

    } else {
      sprite.setTexture(m_boatTexture);

      sprite.setScale(BOAT_SIZE / SPRITE_SIZE);
      sprite.setRotation(m_angle);
    }

    sprite.setPosition(m_position);
    sprite.setAnchor(gf::Anchor::Center);

    target.draw(sprite);
  }
}
