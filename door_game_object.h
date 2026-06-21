#ifndef DOOR_GAME_OBJECT_
#define DOOR_GAME_OBJECT_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class DoorGameObject : public GameObject {

        public:
            DoorGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint tex_closed, GLuint tex_open);

            void SetDoorOpen() { 
                open = true; 
                texture_ = tex_open;
            }

            bool IsOpen() { return open; }

        private:
            bool open;
            GLuint tex_closed;
            GLuint tex_open;

    }; // class BackgroundGameObject

} // namespace game

#endif // DOOR_GAME_OBJECT_