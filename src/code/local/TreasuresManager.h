#ifndef BI_TREASURES_MANAGER_H
#define BI_TREASURES_MANAGER_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Vector.h>

#include "Messages.h"
#include "Treasure.h"

namespace bi {
  class TreasuresManager: public gf::Entity {
  public:
    TreasuresManager();

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

#endif // BI_TREASURES_MANAGER_H
