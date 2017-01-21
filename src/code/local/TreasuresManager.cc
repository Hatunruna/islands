#include "TreasuresManager.h"

#include <gf/VectorOps.h>

#include "Singletons.h"

namespace bi {

  TreasuresManager::TreasuresManager()
  : gf::Entity(5)
  , m_heroPosition({ 0.0f, 0.0f }) {
    // Register message
    gMessageManager().registerHandler<HeroPosition>(&TreasuresManager::onHeroPosition, this);
    gMessageManager().registerHandler<StartScan>(&TreasuresManager::onStartScan, this);
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
      treasure.setHeroPosition(m_heroPosition);
      treasure.update(dt);
    }
  }

  void TreasuresManager::render(gf::RenderTarget& target) {
    for (auto &treasure: m_treasures) {
      treasure.render(target);
    }
  }

  gf::Vector2f TreasuresManager::getNearestTreasure() const {
    const Treasure *treasure = &(m_treasures.front());
    float minDistance = gf::squareDistance(m_heroPosition, treasure->getPosition());

    for (std::size_t i = 1; i < m_treasures.size(); ++i) {
      float distance = gf::squareDistance(m_heroPosition, m_treasures[i].getPosition());
      if (minDistance > distance) {
        minDistance = distance;
        treasure = &(m_treasures[i]);
      }
    }

    return treasure->getPosition() - m_heroPosition;
  }

  gf::MessageStatus TreasuresManager::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    auto hero = static_cast<HeroPosition*>(msg);

    m_heroPosition = hero->position;

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus TreasuresManager::onStartScan(gf::Id id, gf::Message *msg) {
    assert(id == StartScan::type);
    // auto startScan = static_cast<StartScan*>(msg);

    NearestTreasure message;
    message.position = getNearestTreasure();
    gMessageManager().sendMessage(&message);

    return gf::MessageStatus::Keep;
  }
}
