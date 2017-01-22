#ifndef BI_GOLD_H
#define BI_GOLD_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/Texture.h>

#include "Messages.h"

namespace bi {
  class Gold: public gf::Entity {
  public:
    Gold();

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

    bool isGameOver() { return m_gameOver; }

  private:
    gf::MessageStatus onGameOver(gf::Id type, gf::Message *msg);
    gf::MessageStatus onGoldLooted(gf::Id id, gf::Message *msg);

  private:
    unsigned m_score;
    bool m_gameOver;
    gf::Font &m_font;
    gf::Texture &m_coinsTexture;
  };
}

#endif // BI_GOLD_H
