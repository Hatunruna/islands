#ifndef BI_RESOURCEMANAGER_H
#define BI_RESOURCEMANAGER_H

#include <gf/ResourceManager.h>
#include <SFML/Audio.hpp>

namespace {
  template<typename T>
  class ResourceLoader {
  public:
    std::unique_ptr<T> operator()(const gf::Path& filename) {
      std::unique_ptr<T> ptr(new T);
      bool loaded = ptr->loadFromFile(filename.string());
      return loaded ? std::move(ptr) : nullptr;
    }
};

  template<typename T>
  class MusicLoader {
  public:
    std::unique_ptr<T> operator()(const gf::Path& filename) {
      std::unique_ptr<T> ptr(new T);
      bool loaded = ptr->openFromFile(filename.string());
      return loaded ? std::move(ptr) : nullptr;
    }
  };

}

namespace bi{
  class ResourceManager : public gf::ResourceManager {
  public:

    ResourceManager() : gf::ResourceManager(), m_sounds(ResourceLoader<sf::SoundBuffer>()), m_musics(MusicLoader<sf::Music>()) {}

    sf::SoundBuffer& getSound(const gf::Path &path) {
      return m_sounds.getResource(*this, path);
    }

    sf::Music& getMusic(const gf::Path &path) {
      return m_musics.getResource(*this, path);
    }

  private:
    gf::ResourceCache<sf::SoundBuffer> m_sounds;
    gf::ResourceCache<sf::Music> m_musics;
  };
}

#endif // BI_RESOURCEMANAGER_H
