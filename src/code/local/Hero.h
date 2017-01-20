// TODO add licence

#ifndef BI_HERO_H
#define BI_HERO_H

#include <gf/Entity.h>

namespace bi {
  class Hero: public gf::Entity {
  public:
    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
  };
}

#endif // BI_HERO_H
