#ifndef BI_SEA_H
#define BI_SEA_H

#include <gf/Array2D.h>
#include <gf/Entity.h>
#include <gf/Random.h>
#include <gf/VertexArray.h>

namespace bi {

  class Sea : public gf::Entity {
  public:
    static constexpr std::size_t Size = 1000;

    Sea();

    void generate();

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    gf::VertexArray m_vertices;
    gf::Array2D<double> m_sea;
  };

}

#endif // BI_SEA_H
