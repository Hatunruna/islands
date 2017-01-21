#ifndef BI_CONFETTI_H
#define BI_CONFETTI_H

#include <gf/Transformable.h>
#include <gf/Vector.h>
#include <gf/VertexArray.h>

namespace bi {

  class Confetti : public gf::Transformable {
  public:
    Confetti();

    void addCircle(gf::Vector2f position, float radius, gf::Vector4f color);

    virtual void draw(gf::RenderTarget& target, gf::RenderStates states) override;

  private:
    gf::VertexArray m_vertices;
  };

}

#endif // BI_CONFETTI_H
