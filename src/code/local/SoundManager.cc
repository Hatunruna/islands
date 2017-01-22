#include "SoundManager.h"

#include "Singletons.h"

namespace bi {
  SoundManager::SoundManager()
  : m_music(gResourceManager().getMusic("main-music.wav")) {
    // Event

    // Launch the main music
    m_music.play();
    m_music.setLoop(true);
    m_music.setVolume(100.0f);
  }
}
