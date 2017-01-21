#include "TreasureManager.h"

#include <algorithm>

#include <gf/Log.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace bi {
  static constexpr float HITBOX_LIMIT = 60.0f;

  TreasureManager::TreasureManager()
  : gf::Entity(5)
  , m_heroPosition({ 0.0f, 0.0f }) {
    // Register message
    gMessageManager().registerHandler<HeroPosition>(&TreasureManager::onHeroPosition, this);
    gMessageManager().registerHandler<StartScan>(&TreasureManager::onStartScan, this);
  }

  void TreasureManager::addTreasure(const gf::Vector2f position) {
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

  void TreasureManager::update(float dt) {
    for (auto &treasure: m_treasures) {
      treasure.setHeroPosition(m_heroPosition);
      treasure.update(dt);

      float distance = gf::squareDistance(m_heroPosition, treasure.getPosition());
      if (distance <= HITBOX_LIMIT * HITBOX_LIMIT) {
        treasure.found();
        // Sent event
      }
    }

    auto trash = std::partition(m_treasures.begin(), m_treasures.end(), [](Treasure treasure) {
      return !(treasure.isFound());
    });

    for (auto it = trash; it != m_treasures.end(); ++it) {
      assert(it->isFound());
    }

    m_treasures.erase(trash, m_treasures.end());
  }

  void TreasureManager::render(gf::RenderTarget& target) {
    for (auto &treasure: m_treasures) {
      treasure.render(target);
    }
  }

  gf::Vector2f TreasureManager::getNearestTreasure() const {
    auto treasure = std::min_element(std::begin(m_treasures), std::end(m_treasures), [this](const Treasure &a, const Treasure &b){
      float distanceA = gf::squareDistance(m_heroPosition, a.getPosition());
      float distanceB = gf::squareDistance(m_heroPosition, b.getPosition());

      return distanceA < distanceB;
    });

    return treasure->getPosition() - m_heroPosition;
  }

  gf::MessageStatus TreasureManager::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    auto hero = static_cast<HeroPosition*>(msg);

    m_heroPosition = hero->position;

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus TreasureManager::onStartScan(gf::Id id, gf::Message *msg) {
    assert(id == StartScan::type);
    // auto startScan = static_cast<StartScan*>(msg);

    NearestTreasure message;
    message.position = getNearestTreasure();
    gMessageManager().sendMessage(&message);

    return gf::MessageStatus::Keep;
  }
}
