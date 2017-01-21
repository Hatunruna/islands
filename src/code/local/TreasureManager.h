#ifndef BI_TREASURE_MANAGER_H
#define BI_TREASURE_MANAGER_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Vector.h>

#include "Messages.h"
#include "Treasure.h"

namespace bi {
  class TreasureManager: public gf::Entity {
  public:
    TreasureManager();

    void addTreasure(const gf::Vector2f position);

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    gf::Vector2f getNearestTreasure() const;

    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);
    gf::MessageStatus onStartScan(gf::Id id, gf::Message *msg);

  private:
    enum class TreasureType: unsigned {
      BONES,
      NECKLACE,
      GOLD,
      CHEST
    };

  private:
    std::vector<Treasure> m_treasures;
    gf::Vector2f m_heroPosition;
  };
}

#endif // BI_TREASURE_MANAGER_H
