#ifndef BI_MESSAGES_H
#define BI_MESSAGES_H

#include <gf/Message.h>
#include <gf/Vector.h>

using namespace gf::literals;

namespace bi {

  struct HeroPosition: public gf::Message {
    static constexpr gf::Id type = "HeroPosition"_id;

    gf::Vector2f position;
    float elevation;
  };

}

#endif // BI_MESSAGES_H
