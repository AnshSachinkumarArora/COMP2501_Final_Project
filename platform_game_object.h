#ifndef PLATFORM_GAME_OBJECT_H_
#define PLATFORM_GAME_OBJECT_H_

#include "game_object.h"

namespace game {
    class PlatformGameObject : public GameObject {
        public:
            PlatformGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);
    };
}
#endif