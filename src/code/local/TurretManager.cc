#include "TurretManager.h"

#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

#include "Singletons.h"

namespace bi {
  static constexpr float TURRET_SIZE = 76.0f;
  static constexpr float SPRITE_SIZE = 256.0f;

  TurretManager::TurretManager()
  : m_turretTexture(gResourceManager().getTexture("turret.png")) {
    // Event
    gMessageManager().registerHandler<HeroPosition>(&TurretManager::onHeroPosition, this);
  }

  void TurretManager::addTurret(gf::Vector2f position) {
    m_turrets.push_back(Turret(position));
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
    for (auto &turret: m_turrets) {
      gf::Sprite sprite;
      sprite.setTexture(m_turretTexture);
      sprite.setPosition(turret.position);
      sprite.setScale(TURRET_SIZE / SPRITE_SIZE);
      sprite.setAnchor(gf::Anchor::Center);

      target.draw(sprite);
    }
  }

  gf::MessageStatus TurretManager::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    auto hero = static_cast<HeroPosition*>(msg);

    m_heroPosition = hero->position;

    return gf::MessageStatus::Keep;
  }
}
