#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"
#include "player_game_object.h"

namespace game {

    class ParticleSystem;

    enum EnemyState {
        ENEMY_IDLE,
        ENEMY_CHASE,
        ENEMY_ATTACK,
        ENEMY_HURT,
        ENEMY_DEAD
    };

    enum EnemyType {
        MOB,
        TANK,
        BOSS
    };

    class EnemyGameObject : public GameObject {
        public:
            EnemyGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, 
                            GLuint tex_idle, GLuint tex_walk, GLuint tex_attack, 
                            GLuint tex_hurt, GLuint tex_death, PlayerGameObject* target_player, EnemyType type);

            void Update(double delta_time) override;
            void Render(glm::mat4 view_matrix, double current_time) override;

            // Combat Hooks 
            void TakeDamage(int amount, ParticleSystem* particle_system);
            inline bool IsDead() const { return state == ENEMY_DEAD; }
            inline int DealDamage() { return attackPower; }
            inline EnemyState GetState() {return state;}
            int GetHealth() {return hp;}

            //platforming logic
            // Allows the engine to change the floor height dynamically
            inline void SetGround(float new_ground) { ground = new_ground; }
            // Get the height offset for collision
            inline float GetBottomY() { return position_.y - (yscale_ / 2.0f); }

            //particle functions
            inline ParticleSystem* GetParticles(void) { return particle_system; }
            bool FinishedExploding();

            inline EnemyType GetType() { return type; }

        protected:
            PlayerGameObject* target;
            EnemyState state;

            EnemyType type;

            //health
            int hp;
            float i_frames;

            // Physics & Steering
            glm::vec3 velocity;
            float max_speed;
            float max_force; 
            float ground;
            float gravity;

            // AI Radii
            float aggro_radius;
            float attack_radius;

            //attack
            int attackPower;

            // Textures
            GLuint t_idle, t_walk, t_attack, t_hurt, t_death;

            // Animation & State Timers
            bool facing_left;
            float state_timer; // Used to lock the enemy into an attack or hurt animation
            float explosion_timer;
            float attack_duration;
            float hurt_duration;

            // Particle system object
            ParticleSystem *particle_system;
    };

} // namespace game

#endif // ENEMY_GAME_OBJECT_H_