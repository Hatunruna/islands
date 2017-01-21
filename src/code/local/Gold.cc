#include "Gold.h"

#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Text.h>

#include "Singletons.h"

namespace bi {
  Gold::Gold()
  : gf::Entity(10)
  , m_score(0)
  , m_font(gResourceManager().getFont("rm_albion.ttf")) {
    // Event ?
  }

  void Gold::update(float dt) {

  }

  void Gold::render(gf::RenderTarget& target) {
    // Compute the margin 5% of screen
    gf::Vector2f margin;
    margin.x = gWinGeometry().getXRatio(0.95f, 0.0f);
    margin.y = gWinGeometry().getYRatio(0.05f, 0.0f);

    // Compute the font size 10% of height of screen
    float fontSize = 10 * gWinGeometry().getYFromBottom(0.0f) / 100.0f;

    gf::Text text("Gold: " + std::to_string(m_score), m_font, fontSize);
    text.setColor(gf::Color::White);
    text.setOutlineColor(gf::Color::Black);
    text.setAnchor(gf::Anchor::TopRight);
    text.setPosition(margin);

    target.draw(text);
  }
}
