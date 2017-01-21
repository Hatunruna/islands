#include "Confetti.h"

#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

namespace bi {
  static constexpr unsigned CirclePointCount = 30;

  Confetti::Confetti()
  : m_vertices(gf::PrimitiveType::Triangles)
  {

  }

  void Confetti::addCircle(gf::Vector2f position, float radius, gf::Vector4f color) {
    gf::Vector2f prev = { position.x + radius, position.y };

    gf::Vertex vertices[3];
    vertices[0].position = position;
    vertices[0].color = vertices[1].color = vertices[2].color = color;

    for (unsigned i = 1; i <= CirclePointCount; ++i) {
      float angle = i * 2.0f * gf::Pi / CirclePointCount;
      gf::Vector2f curr = position + radius * gf::unit(angle);

      vertices[1].position = prev;
      vertices[2].position = curr;

      m_vertices.append(vertices[0]);
      m_vertices.append(vertices[1]);
      m_vertices.append(vertices[2]);

      prev = curr;
    }
  }

  void Confetti::draw(gf::RenderTarget& target, gf::RenderStates states) {
    states.transform *= getTransform();
    target.draw(m_vertices, states);
  }

}
