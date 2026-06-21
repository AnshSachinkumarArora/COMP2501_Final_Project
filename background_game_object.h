#ifndef BACKGROUND_GAME_OBJECT_
#define BACKGROUND_GAME_OBJECT_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class BackgroundGameObject : public GameObject {

        public:
            BackgroundGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

    }; // class BackgroundGameObject

} // namespace game

#endif // BACKGROUND_GAME_OBJECT_