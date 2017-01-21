#ifndef BI_TURRET_MANAGER_H
#define BI_TURRET_MANAGER_H

#include <queue>
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
      Turret(gf::Vector2f position) : position(position){}

    public:
      struct Bullet {
      public:

      private: // Bullet
      };

    public: // Turret
      gf::Vector2f position;
      std::queue<Bullet> bullets;
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
