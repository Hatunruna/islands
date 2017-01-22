#ifndef BI_DECORATION_MANAGER_H
#define BI_DECORATION_MANAGER_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

#include "Decoration.h"

namespace bi {
    class DecorationManage: public gf::Entity {
    public:
        DecorationManage();
        
        void addDecoration(const gf::Vector2f position);
        
        virtual void update(float dt) override;
        virtual void render(gf::RenderTarget& target) override;
    
    private:
        enum class DecorationType: unsigned {
            ROCKS1,
            ROCKS2,
            PALMTREE1,
            PALMTREE2
        };

    private:
        std::vector<Decoration> m_decorations;
    };
}

#endif // BI_DECORATION_MANAGER_H
