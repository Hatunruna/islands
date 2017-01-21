#ifndef BI_TREASURES_MANAGER_H
#define BI_TREASURES_MANAGER_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Vector.h>

#include "Treasure.h"

namespace bi {
  class TreasuresManager: public gf::Entity {
  public:
    TreasuresManager();

    void addTreasure(const gf::Vector2f position);

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    enum class TreasureType: unsigned {
      BONES,
      NECKLACE,
    };

  private:
    std::vector<Treasure> m_treasures;
  };
}

#endif // BI_TREASURES_MANAGER_H
