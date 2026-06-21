#include "platform_game_object.h"

namespace game {
    PlatformGameObject::PlatformGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture)
        : GameObject(position, geom, shader, texture) {}
}