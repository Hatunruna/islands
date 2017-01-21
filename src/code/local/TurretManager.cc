#include "TurretManager.h"

#include "Singletons.h"

namespace bi {
  TurretManager::TurretManager() {
    // Event
    gMessageManager().registerHandler<HeroPosition>(&TurretManager::onHeroPosition, this);
  }

  void TurretManager::update(float dt) {
    // for (auto &treasure: m_treasures) {
    //   treasure.setHeroPosition(m_heroPosition);
    //   treasure.update(dt);

    //   float distance = gf::squareDistance(m_heroPosition, treasure.getPosition());
    //   if (distance <= HITBOX_LIMIT * HITBOX_LIMIT) {
    //     treasure.found();
    //     // Sent event
    //     GoldLooted message;
    //     message.value = treasure.getValue();
    //     gMessageManager().sendMessage(&message);
    //   }
    // }

    // auto trash = std::partition(m_treasures.begin(), m_treasures.end(), [](Treasure treasure) {
    //   return !(treasure.isFound());
    // });

    // for (auto it = trash; it != m_treasures.end(); ++it) {
    //   assert(it->isFound());
    // }

    // m_treasures.erase(trash, m_treasures.end());
  }

  void TurretManager::render(gf::RenderTarget& target) {
    // for (auto &treasure: m_treasures) {
    //   treasure.render(target);
    // }
  }

  gf::MessageStatus TurretManager::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    auto hero = static_cast<HeroPosition*>(msg);

    m_heroPosition = hero->position;

    return gf::MessageStatus::Keep;
  }
}
