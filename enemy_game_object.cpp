#include "enemy_game_object.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace game {

EnemyGameObject::EnemyGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, 
                                 GLuint tex_idle, GLuint tex_walk, GLuint tex_attack, 
                                 GLuint tex_hurt, GLuint tex_death, PlayerGameObject* target_player, EnemyType type)
    : GameObject(position, geom, shader, tex_idle) {
    
    target = target_player;
    state = ENEMY_IDLE;
    
    t_idle = tex_idle; t_walk = tex_walk; t_attack = tex_attack; 
    t_hurt = tex_hurt; t_death = tex_death;

    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    
    if(type == MOB) {
        // AI Tuning
        max_speed = 2.0f;  
        max_force = 2.0f; 
        aggro_radius = 3.0f;
        attack_radius = 1.2f; // Distance to trigger melee attack
        hp = 2;
        attackPower = 1;
    } else if (type == TANK) {
        max_speed = 2.5f;  
        max_force = 4.0f; 
        aggro_radius = 5.0f;
        attack_radius = 1.2f;
        hp = 4;
        attackPower = 2;
    } else {
        max_speed = 1.5f;  
        max_force = 8.0f; 
        aggro_radius = 8.0f;
        attack_radius = 1.2f;
        hp = 8;
        attackPower = 3;
    }
    
    ground = -4.0f; 
    gravity = 9.8f;
    facing_left = false;

    state_timer = 0.0f;
    attack_duration = 1.0f; // 600ms attack animation
    hurt_duration = 0.5f;
    i_frames = 0.0f;
    explosion_timer = 0.0f;

    particle_system = nullptr;

    this->type = type;
}

void EnemyGameObject::Update(double delta_time) {
    if (state == ENEMY_DEAD) return; // Stop thinking if dead

    glm::vec3 distance_vector = target->GetPosition() - position_;
    float distance_to_player = glm::length(distance_vector);

    // Update facing direction based on player position (so they always look at you)
    if (distance_vector.x < 0.0f && state != ENEMY_IDLE) facing_left = true;
    else facing_left = false;

    //handle iframes
    if(i_frames > 0.0) {
        i_frames -= (float)delta_time;
    }

    // --- STATE MACHINE LOGIC ---
    if(explosion_timer > 0.0f) {
        explosion_timer -= (float)delta_time;
    }
    if (state_timer > 0.0f) {
        // Lock the state if attacking or hurt
        state_timer -= (float)delta_time;
        velocity.x -= velocity.x * 5.0f * (float)delta_time; // Friction while animating
        if (state_timer <= 0.0f) {
            state = ENEMY_IDLE; // Reset when done
        }
    } else {
        // Decide what to do based on distance
        if (distance_to_player <= attack_radius) {
            state = ENEMY_ATTACK;
            state_timer = attack_duration;
        } 
        else if (distance_to_player <= aggro_radius) {
            state = ENEMY_CHASE;
        } 
        else {
            state = ENEMY_IDLE;
        }
    }

    // --- STEERING BEHAVIOR (CHASE) ---
    if (state == ENEMY_CHASE) {
        glm::vec3 desired_velocity = glm::normalize(distance_vector) * max_speed;
        desired_velocity.y = 0.0f; // Don't fly

        glm::vec3 steering = desired_velocity - velocity;
        steering.y = 0.0f;

        velocity += steering * max_force * (float)delta_time;
    } else if (state == ENEMY_IDLE) {
        // Friction to stop
        velocity.x -= velocity.x * 5.0f * (float)delta_time;
    }

    // --- PHYSICS ---
    velocity.y -= gravity * (float)delta_time;
    position_ += velocity * (float)delta_time;

    if (position_.y <= ground) {
        position_.y = ground;
        velocity.y = 0.0f;
    }

    GameObject::Update(delta_time);
}

void EnemyGameObject::Render(glm::mat4 view_matrix, double current_time) {
    int num_frames = 1;
    int current_frame = 0;
    float anim_speed = 8.0f;

    // --- ANIMATION STATE SELECTOR ---
    switch (state) {
        case ENEMY_IDLE:
            texture_ = t_idle; num_frames = 4; anim_speed = 4.0f;
            current_frame = (int)(current_time * anim_speed) % num_frames;
            break;
        case ENEMY_CHASE:
            texture_ = t_walk; 
            (type == MOB || type == TANK) ? num_frames = 6 : num_frames = 4;
            anim_speed = 8.0f;
            current_frame = (int)(current_time * anim_speed) % num_frames;
            break;
        case ENEMY_ATTACK:
            texture_ = t_attack; 
            if (type == MOB) {
                num_frames = 6;
            } else if (type == TANK) {
                num_frames = 9;
            } else {
                num_frames = 4;
            }
            // Play once based on state_timer
            current_frame = std::clamp((int)((1.0f - (state_timer / attack_duration)) * num_frames), 0, num_frames - 1);
            break;
        case ENEMY_HURT:
            texture_ = t_hurt; num_frames = 2;
            current_frame = std::clamp((int)((1.0f - (state_timer / hurt_duration)) * num_frames), 0, num_frames - 1);
            break;
        case ENEMY_DEAD:
            texture_ = t_death; 
            (type == MOB || type == TANK) ? num_frames = 6 : num_frames = 4;
            // Freeze on the last frame of the death animation
            current_frame = num_frames - 1; 
            break;
    }

    // --- UV & MATRIX MATH ---
    glm::vec2 uv_scale = glm::vec2(1.0f / (float)num_frames, 1.0f);
    glm::vec2 uv_offset = glm::vec2(current_frame * uv_scale.x, 0.0f);

    shader_->Enable();
    shader_->SetUniformMat4("view_matrix", view_matrix);
    shader_->SetUniform2f("uv_scale", uv_scale);
    shader_->SetUniform2f("uv_offset", uv_offset);

    float flip_scale = facing_left ? -xscale_ : xscale_;
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(flip_scale, yscale_, 1.0));
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);
    
    glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;
    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

    geometry_->SetGeometry(shader_->GetShaderProgram());
    glBindTexture(GL_TEXTURE_2D, texture_);
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}

bool EnemyGameObject::FinishedExploding() {
    if(explosion_timer <= 0.0f) {
        delete particle_system;
        particle_system = nullptr;
        return true;
    }
    return false;
}

void EnemyGameObject::TakeDamage(int amount, ParticleSystem* particle_system) {
    if (state == ENEMY_DEAD || state == ENEMY_HURT || i_frames > 0.0) return; 

    this->particle_system = particle_system;
    
    hp -= amount; 
    
    if (hp <= 0) { 
        state = ENEMY_DEAD; 
    } 
    else {
        state = ENEMY_HURT;
        state_timer = hurt_duration; // Interrupt and play hurt animation
        explosion_timer = 1.0;
        i_frames = 1.0f;
    }
}

} // namespace game