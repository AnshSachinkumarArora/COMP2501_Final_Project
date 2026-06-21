#include "collectible_game_object.h"

namespace game {

CollectibleGameObject::CollectibleGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, CollectibleType type)
    : GameObject(position, geom, shader, texture) {
    type_ = type;
    is_collected_ = false;
    base_y = position.y; 
    parametric_timer = 0.0f;
}

// --- PARAMETRIC MOTION ---
void CollectibleGameObject::Update(double delta_time) {
    if (!is_collected_) {
        // 1. Advance the time parameter (t)
        parametric_timer += (float)delta_time;
        
        // 2. Define the parametric curve constraints
        float amplitude = 0.2f; // How high/low it bobs
        float speed = 3.0f;     // How fast it bobs
        
        // 3. Apply the parametric equation to the Y axis
        position_.y = base_y + std::sin(parametric_timer * speed) * amplitude;
    }
    
    GameObject::Update(delta_time);
}

void CollectibleGameObject::Render(glm::mat4 view_matrix, double current_time) {
    // If it's already picked up, do not draw it!
    if (!is_collected_) {
        GameObject::Render(view_matrix, current_time);
    }
}

} // namespace game