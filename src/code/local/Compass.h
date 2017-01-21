#ifndef BI_COMPASS_H
#define BI_COMPASS_H

#include <gf/Entity.h>
#include <gf/Texture.h>

#include "Messages.h"

namespace bi {
  class Compass: public gf::Entity {
  public:
    Compass();

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

    gf::MessageStatus onStartScan(gf::Id id, gf::Message *msg);
    gf::MessageStatus onNearestTreasure(gf::Id id, gf::Message *msg);

  private:
    float m_perfectAngleTarget;
    float m_estimatedAngleTarget;
    float m_currentAngleTarget;
    float m_stepAngle;
    unsigned m_currentStep;
    float m_timeElapsed;
    bool m_displayed;
    gf::Texture &m_compassTexture;
    gf::Texture &m_pointerTexture;
  };
}

#endif // BI_COMPASS_H
