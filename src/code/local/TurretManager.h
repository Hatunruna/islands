#ifndef BI_TURRET_MANAGER_H
#define BI_TURRET_MANAGER_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Vector.h>

#include "Messages.h"

namespace bi {
  class TurretManager: public gf::Entity {
  public:
    TurretManager();

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    struct Turret {
    public:

    private:
      struct Bullet {
      public:

      private:
      };
    };

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);

  private:
    std::vector<Turret> m_turrets;
    gf::Vector2f m_heroPosition;

  };
}

#endif // BI_TURRET_MANAGER_H
