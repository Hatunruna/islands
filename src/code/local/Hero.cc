#include "Hero.h"

#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

#include <gf/Log.h>

namespace bi {
  static constexpr float ANGULAR_VELOCITY = 10.0f;
  static constexpr float VELOCITY = 10.0f;

  Hero::Hero(const gf::Vector2f postion)
  : m_turn(Turn::NONE)
  , m_move(Move::NONE)
  , m_position(postion)
  , m_angle(0.0f) {

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

  void Hero::update(float dt) {
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

    // gf::Log::print("%f - %f", m_angle, velocity);
    m_position += gf::unit(m_angle) * velocity;
    // gf::Log::print("%f - %f\n", m_position.x, m_position.y);
  }

  void Hero::render(gf::RenderTarget& target) {
    gf::CircleShape sprite(10.0f, 3);
    sprite.setColor(gf::Color::Red);
    sprite.setPosition(m_position);
    sprite.setRotation(m_angle + gf::Pi2);
    sprite.setAnchor(gf::Anchor::Center);

    target.draw(sprite);
  }
}
