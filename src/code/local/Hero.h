// TODO add licence

#ifndef BI_HERO_H
#define BI_HERO_H

#include <gf/Entity.h>
#include <gf/Vector.h>

namespace bi {
  class Hero: public gf::Entity {
  public:
    enum class Turn {
      LEFT,
      RIGHT,
      NONE,
    };

    enum class Move {
      FORWARD,
      BACKWARD,
      NONE,
    };

  public:
    Hero(const gf::Vector2f postion);

    void moveForward();
    void moveBackward();
    void moveStop();

    void turnLeft();
    void turnRight();
    void turnNone();

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    Turn m_turn;
    Move m_move;
    gf::Vector2f m_position;
    float m_angle;
  };
}

#endif // BI_HERO_H
