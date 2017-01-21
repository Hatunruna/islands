#include "TreasuresManager.h"

#include "Singletons.h"

namespace bi {

  TreasuresManager::TreasuresManager()
  : gf::Entity(10) {
    // Events ?
  }

  void TreasuresManager::addTreasure(const gf::Vector2f position) {
    // Choose a random treasure
    static constexpr unsigned NUMBER_TREASURE = 4;
    TreasureType treasureType = static_cast<TreasureType>(gRandom().computeUniformInteger<unsigned>(0, NUMBER_TREASURE-1));

    switch (treasureType) {
    case TreasureType::BONES:
      m_treasures.push_back(Treasure(position, 40.0f, "bones.png"));
      break;

    case TreasureType::NECKLACE:
      m_treasures.push_back(Treasure(position, 40.0f, "relics_01.png"));
      break;

    case TreasureType::GOLD:
      m_treasures.push_back(Treasure(position, 80.0f, "relics_02.png"));
      break;

    case TreasureType::CHEST:
      m_treasures.push_back(Treasure(position, 80.0f, "chest.png"));
    }
  }

  void TreasuresManager::update(float dt) {
    for (auto &treasure: m_treasures) {
      treasure.update(dt);
    }
  }

  void TreasuresManager::render(gf::RenderTarget& target) {
    for (auto &treasure: m_treasures) {
      treasure.render(target);
    }
  }
}
