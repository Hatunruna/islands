#ifndef BI_TREASURE_H
#define BI_TREASURE_H

#include <gf/Entity.h>
#include <gf/Texture.h>

namespace bi {
  class Treasure: public gf::Entity {
  public:
    Treasure(const gf::Vector2f postion, const float sizeRendered, const gf::Path texturePath);

    gf::Vector2f getPosition() const;
    void setHeroPosition(const gf::Vector2f heroPosition);

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    gf::Texture &m_texture;
    gf::Vector2f m_position;
    float m_SIZE_RENDERED;
    float m_alphaLevel;
    gf::Vector2f m_heroPosition;
  };
}

#endif // BI_TREASURE_H
