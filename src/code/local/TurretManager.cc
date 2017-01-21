#include "TurretManager.h"

#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace bi {
  static constexpr float TURRET_SIZE = 76.0f;
  static constexpr float SPRITE_SIZE = 256.0f;
  static constexpr float RADUIS_TARGET_TURRET = 350.0f;
  static constexpr float COOLDOWN_FIRE = 1.0f;

  TurretManager::TurretManager()
  : m_turretTexture(gResourceManager().getTexture("turret.png")) {
    // Event
    gMessageManager().registerHandler<HeroPosition>(&TurretManager::onHeroPosition, this);
  }

  void TurretManager::addTurret(gf::Vector2f position) {
    Turret turret;
    turret.position = position;
    turret.timeElapsed = 0.0f;
    m_turrets.push_back(turret);
  }

  void TurretManager::update(float dt) {
    for (auto &turret: m_turrets) {
      // Update the bullet
      for (auto &bullet: turret.bullets) {
        bullet.position += bullet.velocity * dt;
        bullet.timeElapsed += dt;
        if (bullet.timeElapsed >= 6.0f) {
          bullet.active = false;
        }
      }

      // Check if the turret has spoted the hero
      turret.timeElapsed += dt;
      if (turret.timeElapsed >= COOLDOWN_FIRE) {
        turret.timeElapsed -= COOLDOWN_FIRE;
        float distance =  gf::squareDistance(m_heroPosition, turret.position);
        if (distance <= RADUIS_TARGET_TURRET * RADUIS_TARGET_TURRET) {
          // Create the bullet
          Turret::Bullet bullet;
          bullet.position = turret.position;
          bullet.velocity = m_heroPosition - turret.position;
          bullet.active = true;
          bullet.timeElapsed = 0.0f;

          turret.bullets.push_back(bullet);
        }
      }

      // Remove the last bullet
      auto trash = std::partition(turret.bullets.begin(), turret.bullets.end(), [](Turret::Bullet &bullet) {
        return bullet.active;
      });

      for (auto it = trash; it != turret.bullets.end(); ++it) {
        assert(!it->active);
      }

      turret.bullets.erase(trash, turret.bullets.end());
    }
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
      // Draw the turret
      gf::Sprite sprite;
      sprite.setTexture(m_turretTexture);
      sprite.setPosition(turret.position);
      sprite.setScale(TURRET_SIZE / SPRITE_SIZE);
      sprite.setAnchor(gf::Anchor::Center);

      target.draw(sprite);

      // Draw the bullet
      for (auto &bullet: turret.bullets) {
        gf::CircleShape shape;
        shape.setRadius(2.0f);
        shape.setColor(gf::Color::Red);
        shape.setPosition(bullet.position);
        shape.setAnchor(gf::Anchor::Center);
        target.draw(shape);
      }
    }
  }

  gf::MessageStatus TurretManager::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    auto hero = static_cast<HeroPosition*>(msg);

    m_heroPosition = hero->position;

    return gf::MessageStatus::Keep;
  }
}
