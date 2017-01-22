#include "DecorationManager.h"

#include <gf/Log.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace bi {
  DecorationManage::DecorationManage()
  : gf::Entity(5) {
  }

  void DecorationManage::addDecoration(const gf::Vector2f position) {
    // Choose a random treasure
    static constexpr unsigned NUMBER_DECORATION = 4;
    DecorationType decorationType = static_cast<DecorationType>(gRandom().computeUniformInteger<unsigned>(0, NUMBER_DECORATION-1));

    switch (decorationType) {
    case DecorationType::ROCKS1:
      m_decorations.push_back(Decoration(position, 40.0f, "rocks_01.png"));
      break;

    case DecorationType::ROCKS2:
      m_decorations.push_back(Decoration(position, 40.0f, "rocks_02.png"));
      break;

    case DecorationType::PALMTREE1:
      m_decorations.push_back(Decoration(position, 80.0f, "palm_tree01.png"));
      break;

    case DecorationType::PALMTREE2:
      m_decorations.push_back(Decoration(position, 80.0f, "palm_tree02.png"));
    }
  }

  void DecorationManage::update(float dt) {
    for (auto &decoration : m_decorations) {
        decoration.update(dt);
    }
  }

  void DecorationManage::render(gf::RenderTarget& target) {
    for (auto &decoration: m_decorations) {
      decoration.render(target);
    }
  }
}
