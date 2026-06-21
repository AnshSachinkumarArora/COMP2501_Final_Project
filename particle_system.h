#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class ParticleSystem : public GameObject {

        public:
            ParticleSystem(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GameObject *parent,
            bool is_blood);

            void Update(double delta_time) override;

            void Render(glm::mat4 view_matrix, double current_time) override;

        private:
            GameObject *parent_;

            bool is_blood;

    }; // class ParticleSystem

} // namespace game

#endif // PARTICLE_SYSTEM_H_
