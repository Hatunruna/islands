#include "Sea.h"

#include <gf/ColorRamp.h>
#include <gf/Noises.h>
#include <gf/RenderTarget.h>

#include "Singletons.h"

namespace bi {

  static constexpr double Scale = 12.0;
  static constexpr double SeaLevel = 0.65;

  static constexpr float TileSize = 16.0f;

  Sea::Sea()
  : m_vertices(gf::PrimitiveType::Triangles)
  , m_sea({ Size, Size })
  {

  }

  static double valueWithWaterLevel(double value, double waterLevel) {
    if (value < waterLevel) {
      return value / waterLevel * 0.5;
    }

    return (value - waterLevel) / (1.0 - waterLevel) * 0.5 + 0.5;
  }

  void Sea::generate() {
    // generate the elevation

    gf::SimplexNoise2D simplex(gRandom());
    gf::FractalNoise2D fractal(simplex, 1);

    for (auto row : m_sea.getRowRange()) {
      double y = static_cast<double>(row) / m_sea.getRows() * Scale;
      for (auto col : m_sea.getColRange()) {
        double x = static_cast<double>(col) / m_sea.getCols() * Scale;
        m_sea({ col, row }) = fractal.getValue(x, y);
      }
    }

    // normalize

    double min = *std::min_element(m_sea.begin(), m_sea.end());
    double max = *std::max_element(m_sea.begin(), m_sea.end());

    for (auto& val : m_sea) {
      val = (val - min) / (max - min);
      assert(0.0 <= val && val <= 1.0);
    }

    // TODO: edges and cut islands



    // compute the vertices

    // see: http://www.blitzbasic.com/codearcs/codearcs.php?code=2415
    gf::ColorRamp ramp;
    ramp.addColorStop(0.000f, gf::Color::fromRgba32(  2,  43,  68)); // very dark blue: deep water
    ramp.addColorStop(0.250f, gf::Color::fromRgba32(  9,  62,  92)); // dark blue: water
    ramp.addColorStop(0.490f, gf::Color::fromRgba32( 17,  82, 112)); // blue: shallow water
    ramp.addColorStop(0.500f, gf::Color::fromRgba32( 69, 108, 118)); // light blue: shore
    ramp.addColorStop(0.501f, gf::Color::fromRgba32( 42, 102,  41)); // green: grass
    ramp.addColorStop(0.750f, gf::Color::fromRgba32(115, 128,  77)); // light green: veld
    ramp.addColorStop(0.850f, gf::Color::fromRgba32(153, 143,  92)); // brown: tundra
    ramp.addColorStop(0.950f, gf::Color::fromRgba32(179, 179, 179)); // grey: rocks
    ramp.addColorStop(1.000f, gf::Color::fromRgba32(255, 255, 255)); // white: snow

    for (auto row : m_sea.getRowRange()) {
      if (row == Size - 1) {
        continue;
      }

      for (auto col : m_sea.getColRange()) {
        if (col == Size - 1) {
          continue;
        }

        double valNW = valueWithWaterLevel(m_sea({ row,     col     }), SeaLevel);
        double valNE = valueWithWaterLevel(m_sea({ row,     col + 1 }), SeaLevel);
        double valSW = valueWithWaterLevel(m_sea({ row + 1, col     }), SeaLevel);
        double valSE = valueWithWaterLevel(m_sea({ row + 1, col + 1 }), SeaLevel);

        gf::Vertex vertices[4];

        vertices[0].position = {  col      * TileSize,  row      * TileSize };
        vertices[1].position = { (col + 1) * TileSize,  row      * TileSize };
        vertices[2].position = {  col      * TileSize, (row + 1) * TileSize };
        vertices[3].position = { (col + 1) * TileSize, (row + 1) * TileSize };

        vertices[0].color = ramp.computeColor(valNW);
        vertices[1].color = ramp.computeColor(valNE);
        vertices[2].color = ramp.computeColor(valSW);
        vertices[3].color = ramp.computeColor(valSE);

        // first triangle
        m_vertices.append(vertices[0]);
        m_vertices.append(vertices[1]);
        m_vertices.append(vertices[2]);

        // second triangle
        m_vertices.append(vertices[2]);
        m_vertices.append(vertices[1]);
        m_vertices.append(vertices[3]);

      }
    }



  }

  void Sea::update(float dt) {

  }

  void Sea::render(gf::RenderTarget& target) {
    target.draw(m_vertices);
  }

}
