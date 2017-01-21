#ifndef BI_COMPASS_H
#define BI_COMPASS_H

#include <gf/Entity.h>

namespace bi {
  class Compass: public gf::Entity {
  public:
    Compass();

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    float m_angle;
  };
}

#endif // BI_COMPASS_H
