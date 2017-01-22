#ifndef BI_GOLD_H
#define BI_GOLD_H

#include <gf/Entity.h>
#include <gf/Font.h>

#include "Messages.h"

namespace bi {
  class Gold: public gf::Entity {
  public:
    Gold();

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    gf::MessageStatus onGoldLooted(gf::Id id, gf::Message *msg);

  private:
    unsigned m_score;
    gf::Font &m_font;
  };
}

#endif // BI_GOLD_H
