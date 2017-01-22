#ifndef BI_DECORATION_H
#define BI_DECORATION_H

#include <gf/Entity.h>
#include <gf/Texture.h>

namespace bi {
  class Decoration: public gf::Entity {
  public:
    Decoration(const gf::Vector2f postion, const float sizeRendered, float rotate, const gf::Path texturePath);

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    gf::Texture *m_texture;
    gf::Vector2f m_position;
    float m_SIZE_RENDERED;
    float m_rotation;
  };
}

#endif // BI_DECORATION_H
