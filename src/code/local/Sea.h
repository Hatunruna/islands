#ifndef BI_SEA_H
#define BI_SEA_H

#include <gf/Array2D.h>
#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/Random.h>
#include <gf/VertexArray.h>

namespace bi {

  class Sea : public gf::Entity {
  public:
    Sea();

    void generate();

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);

  private:
    struct Point {
      float elevation;
      gf::Color4f color;
    };

    gf::VertexArray m_vertices;
    gf::Array2D<Point> m_sea;
    gf::Vector2u m_hero;
  };

}

#endif // BI_SEA_H
