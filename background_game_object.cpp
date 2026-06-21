#include "background_game_object.h"

namespace game {

//custom child game object class for background

BackgroundGameObject::BackgroundGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture)
	: GameObject(position, geom, shader, texture) {}

}