#include "Gold.h"

#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

#include "Singletons.h"

namespace bi {
  static constexpr float SPRITE_SIZE = 256.0f;
  static constexpr float SIZE_RENDERED = 30.0f;

  Gold::Gold()
  : gf::Entity(10)
  , m_score(0)
  , m_gameOver(false)
  , m_win(false)
  , m_font(gResourceManager().getFont("rm_albion.ttf"))
  , m_coinsTexture(gResourceManager().getTexture("coin.png")) {
    // Event
    gMessageManager().registerHandler<GameOver>(&Gold::onGameOver, this);
    gMessageManager().registerHandler<GoldLooted>(&Gold::onGoldLooted, this);
  }

  void Gold::update(float dt) {
    // Nothing
  }

  void Gold::render(gf::RenderTarget& target) {
    if (m_gameOver || m_win) {
      // Compute the margin 50% of screen
      gf::Vector2f margin;
      margin.x = gWinGeometry().getXRatio(0.5f, 0.0f);
      margin.y = gWinGeometry().getYRatio(0.5f, 0.0f);

      // Compute the font size 10% of height of screen
      float fontSize = 150.0f;

      if (m_win) {
        gf::Text text("You win!\nYour score is : " + std::to_string(m_score), m_font, fontSize);
        text.setColor(gf::Color::White);
        text.setOutlineColor(gf::Color::Black);
        text.setAnchor(gf::Anchor::Center);
        text.setPosition(margin);

        target.draw(text);
      } else {
        gf::Text text("Game Over!\nYour score is : " + std::to_string(m_score), m_font, fontSize);
        text.setColor(gf::Color::White);
        text.setOutlineColor(gf::Color::Black);
        text.setAnchor(gf::Anchor::Center);
        text.setPosition(margin);

        target.draw(text);
      }
    } else {
      // Compute the margin 5% of screen
      gf::Vector2f margin;
      margin.x = gWinGeometry().getXRatio(0.95f, 0.0f);
      margin.y = gWinGeometry().getYRatio(0.05f, 0.0f);

      // Compute the font size 10% of height of screen
      float fontSize = 10 * gWinGeometry().getYFromBottom(0.0f) / 100.0f;

      gf::Text text(std::to_string(m_score), m_font, fontSize);
      text.setColor(gf::Color::White);
      text.setOutlineColor(gf::Color::Black);
      text.setAnchor(gf::Anchor::TopRight);
      text.setPosition(margin);

      target.draw(text);

      // Draw the coins
      margin.x -= text.getLocalBounds().width + gWinGeometry().getXRatio(0.01f, 0.0f);
      margin.y -= text.getLocalBounds().height;
      gf::Sprite sprite;
      sprite.setTexture(m_coinsTexture);
      sprite.setPosition(margin);
      sprite.setScale(fontSize / SPRITE_SIZE);
      sprite.setAnchor(gf::Anchor::TopRight);

      target.draw(sprite);
    }
  }

  gf::MessageStatus Gold::onGoldLooted(gf::Id id, gf::Message *msg) {
    assert(id == GoldLooted::type);
    auto gold = static_cast<GoldLooted*>(msg);

    m_score += gold->value;
    if (m_score > 5000) {
      m_win = true;
    }

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Gold::onGameOver(gf::Id id, gf::Message *msg) {
    assert(id == GameOver::type);

    m_gameOver = true;

    return gf::MessageStatus::Keep;
  }

}
