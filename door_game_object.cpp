#include "door_game_object.h"

namespace game {

//custom child game object class for background

DoorGameObject::DoorGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint tex_closed, GLuint tex_open)
	: GameObject(position, geom, shader, tex_closed) {
        this->tex_closed = tex_closed;
        this->tex_open = tex_open;
        open = false;
    }

}