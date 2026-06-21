#ifndef COLLECTIBLE_GAME_OBJECT_H_
#define COLLECTIBLE_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    enum CollectibleType {
        TYPE_SPEED_BOOST,
        TYPE_SWORD,
        TYPE_KEY
    };

    class CollectibleGameObject : public GameObject {
        public:
            CollectibleGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, CollectibleType type);

            void Update(double delta_time) override;

            // Getters and Setters
            inline CollectibleType GetType(void) const { return type_; }
            inline bool IsCollected(void) const { return is_collected_; }
            inline void SetCollected(bool collected) { is_collected_ = collected; }

            // Override render so it disappears when collected
            void Render(glm::mat4 view_matrix, double current_time) override;

        private:
            CollectibleType type_;
            bool is_collected_;

            //parametric motion
            float base_y; 
            float parametric_timer;
    };
}
#endif