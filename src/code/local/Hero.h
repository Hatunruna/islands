// TODO add licence

#ifndef BI_HERO_H
#define BI_HERO_H

#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

#include "Messages.h"
#include "Steam.h"

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
    Hero(Steam& steam, const gf::Vector2f postion);

    void moveForward();
    void moveBackward();
    void moveStop();

    void turnLeft();
    void turnRight();
    void turnNone();

    void sendInitialPosition() const;
    bool isOnIsland() const;
    bool scanAvailable() const;

    gf::MessageStatus onStartScan(gf::Id id, gf::Message *msg);
    gf::MessageStatus onStopScan(gf::Id id, gf::Message *msg);

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    Steam& m_steam;

    Turn m_turn;
    Move m_move;
    gf::Vector2f m_position;
    float m_angle;

    gf::Texture &m_hatTexture;
    gf::Texture &m_boatTexture;

    float m_timeElapsed;
    bool m_alternateStep;
    bool m_isOnIsland;
    bool m_isFrozen;
  };
}

#endif // BI_HERO_H
