#ifndef BI_WAVE_MANAGER_H
#define BI_WAVE_MANAGER_H

#include <gf/Entity.h>

namespace bi {

  class WaveManager : public gf::Entity {
  public:

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:

  };

}


#endif // BI_WAVE_MANAGER_H
