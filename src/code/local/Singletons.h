#ifndef BI_SINGLETONS_H
#define BI_SINGLETONS_H

#include <gf/MessageManager.h>
#include <gf/Random.h>
#include <gf/Singleton.h>
#include <gf/ResourceManager.h>
#include <gf/WindowGeometryTracker.h>

#include "ResourceManager.h"

namespace bi {
  extern gf::Singleton<gf::MessageManager> gMessageManager;
  extern gf::Singleton<ResourceManager> gResourceManager;
  extern gf::Singleton<gf::Random> gRandom;
  extern gf::Singleton<gf::WindowGeometryTracker> gWinGeometry;
}

#endif // BI_SINGLETONS_H
