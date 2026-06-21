#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GLuint tex_jump,
            GLuint tex_idle, GLuint tex_run, GLuint tex_attack, GLuint tex_knife, GLuint tex_sword, 
                            GLuint tex_hurt, GLuint tex_death);

            // Update function for moving the player object around
            void Update(double delta_time) override;

            // New player render function for animation
            void Render(glm::mat4 view_matrix, double current_time) override;

            //melee attack logic
            void Attack();
            inline bool isAttacking() {return is_attacking;}
            inline int DealDamage() { 
                if (current_weapon == tex_knife) {return 1;}
                else {return 2;}    
            }

            //player physics based motion logic
            inline glm::vec3 GetVelocity(void) {return velocity;}
            inline void SetVelocity(const glm::vec3 vel) {velocity = vel;}
            inline bool InAir(void) {return in_air;}
            void Jump(void);

            //collectible logic
            void UnlockSword() { 
                has_sword = true;
                current_weapon = tex_sword;    
            }
            void GiveKey() { has_key = true; }
            void ActivateSpeedBoost() { speed_timer = 5.0f; }
            float SpeedTimer() {return speed_timer;}
            bool HasKey() {return has_key;}
            bool HasSword() { return has_sword; }
            void SwitchWeapon();

            //platforming logic
            // Allows the engine to change the floor height dynamically
            inline void SetGround(float new_ground) { ground = new_ground; }
            // Get the height offset for collision
            inline float GetBottomY() { return position_.y - (yscale_ / 2.0f); }
            inline bool IsStunned() { return stun_timer > 0.0f; }

            //hp logic
            void TakeDamage(int damage) {
                // Ignore damage if the player is already hurt (i-frames) or dead
                if (took_damage || hp <= 0 || i_frames > 0.0) return;
                hurt_timer = hurt_duration;
                attack_timer = 0.0;
                is_attacking = false; // Cancel current attack
                hp -= damage;
                took_damage = true;
                i_frames = 2.0f;
            }
            bool isDead() {return (hp <= 0);}
            int GetHealth() {return hp;}

        protected:
            //physics
            glm::vec3 velocity;

            //health
            int hp;
            bool took_damage;
            float hurt_timer;
            float hurt_duration;

            // texture
            GLuint tex_jump;
            GLuint tex_idle;
            GLuint tex_run;
            GLuint tex_attack;
            GLuint tex_knife;
            GLuint tex_sword;
            GLuint tex_hurt;
            GLuint tex_death;
            GLuint current_weapon;

            //jump logic
            bool in_air;
            float ground;
            float restitution;
            float gravity;
            float stun_timer;

            //melee attacking logic
            bool is_attacking;
            float attack_timer;
            float attack_duration;
            bool facing_left;
            float i_frames;

            //collectible logic
            bool has_sword = false;
            bool has_key = false;
            float speed_timer = 0.0f;

    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
