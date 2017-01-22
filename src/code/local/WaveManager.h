#ifndef BI_WAVE_MANAGER_H
#define BI_WAVE_MANAGER_H

#include <deque>

#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/Vector.h>

#include <SFML/Audio.hpp>

namespace bi {

  class WaveManager : public gf::Entity {
  public:
    WaveManager();

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);

  private:
    struct Bubble {
      gf::Vector2f position;
      float angle;
      float radius;
      float lifetime;
    };

    std::deque<Bubble> m_bubbles;

    gf::Vector2f m_p0;
    gf::Vector2f m_p1;

    float m_elapsed;

    gf::Vector2f m_hero;
    sf::Sound m_waveSound;
  };

}


#endif // BI_WAVE_MANAGER_H
