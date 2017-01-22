#ifndef BI_TURRET_MANAGER_H
#define BI_TURRET_MANAGER_H

#include <deque>
#include <vector>

#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

#include "Messages.h"

namespace bi {
  class TurretManager: public gf::Entity {
  public:
    TurretManager();

    void addTurret(gf::Vector2f position);

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    struct Turret {
    public:
      struct Bullet {
        gf::Vector2f position;
        gf::Vector2f velocity;
        float timeElapsed;
        bool active;
      };

    public: // Turret
      gf::Vector2f position;
      std::deque<Bullet> bullets;
      float timeElapsed;
    };

  private: // TurretManager
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);

  private:
    std::vector<Turret> m_turrets;
    gf::Vector2f m_heroPosition;
    gf::Texture &m_turretTexture;
  };
}

#endif // BI_TURRET_MANAGER_H
