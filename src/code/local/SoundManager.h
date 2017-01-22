#ifndef BI_SOUND_MANAGER
#define BI_SOUND_MANAGER

#include <SFML/Audio.hpp>

namespace bi {
  class SoundManager {
  public:
    SoundManager();

  private:
    sf::Music &m_music;
  };
}

#endif // BI_SOUND_MANAGER
