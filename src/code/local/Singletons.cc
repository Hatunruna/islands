#include "Singletons.h"

namespace bi {
  gf::Singleton<gf::MessageManager> gMessageManager;
  gf::Singleton<ResourceManager> gResourceManager;
  gf::Singleton<gf::Random> gRandom;
  gf::Singleton<gf::WindowGeometryTracker> gWinGeometry;
}

