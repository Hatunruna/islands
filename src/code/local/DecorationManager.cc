#include "DecorationManager.h"

#include <gf/Log.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace bi {
  DecorationManager::DecorationManager(int priority)
  : gf::Entity(priority) {
  }

  void DecorationManager::addDecoration(const gf::Vector2f position) {
    // Choose a random treasure
    static constexpr unsigned NUMBER_DECORATION = 6;
    DecorationType decorationType = static_cast<DecorationType>(gRandom().computeUniformInteger<unsigned>(0, NUMBER_DECORATION-1));
    float angle = gRandom().computeUniformFloat<float>(0, gf::Pi * 2);

    if(getPriority() > 10) {
      // ABOVE HERO
      switch (decorationType) {
      case DecorationType::ROCKS1:
      case DecorationType::PALMTREE1:
        m_decorations.push_back(Decoration(position, 160.0f, angle, "palm_tree01.png"));
        break;

      case DecorationType::ROCKS2:
      case DecorationType::PALMTREE2:
        m_decorations.push_back(Decoration(position, 160.0f, angle, "palm_tree02.png"));
        break;
      case DecorationType::CAMPFIRE:
      case DecorationType::PALMTREE3:
        m_decorations.push_back(Decoration(position, 160.0f, angle, "palm_tree03.png"));
        break;
      }
    } else {
      // BELOW HERO
      switch (decorationType) {
      case DecorationType::ROCKS1:
      case DecorationType::PALMTREE1:
        m_decorations.push_back(Decoration(position, 80.0f, angle, "rocks_01.png"));
        break;

      case DecorationType::ROCKS2:
      case DecorationType::PALMTREE2:
        m_decorations.push_back(Decoration(position, 80.0f, angle, "rocks_02.png"));
        break;

      case DecorationType::CAMPFIRE:
      case DecorationType::PALMTREE3:
        m_decorations.push_back(Decoration(position, 100.0f, angle, "campfire.png"));
        break;
      }

    }
  }

  void DecorationManager::update(float dt) {
    for (auto &decoration : m_decorations) {
        decoration.update(dt);
    }
  }

  void DecorationManager::render(gf::RenderTarget& target) {
    for (auto &decoration: m_decorations) {
      decoration.render(target);
    }
  }
}
