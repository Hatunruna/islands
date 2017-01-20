#ifndef BI_SEA_H
#define BI_SEA_H

#include <gf/Entity.h>

namespace bi {

  class Sea : public gf::Entity {
  public:

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:

  };

}

#endif // BI_SEA_H
