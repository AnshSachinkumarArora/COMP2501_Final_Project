#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "player_game_object.h"
#include <algorithm>

namespace game {

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GLuint tex_jump,
            GLuint tex_idle, GLuint tex_run, GLuint tex_attack, GLuint tex_knife, GLuint tex_sword, 
                            GLuint tex_hurt, GLuint tex_death)
	: GameObject(position, geom, shader, texture) {
		velocity = glm::vec3(0.0,0.0,0.0);
		in_air = false;
		ground = -4.0f;
		restitution = 0.5f;
		gravity = 9.8f;
		this->tex_jump = tex_jump;
		this->tex_idle = tex_idle;
		this->tex_run = tex_run;
        this->tex_attack = tex_attack;
        this->tex_knife = tex_knife;
        this->tex_sword = tex_sword;
        this->tex_hurt = tex_hurt;
        this->tex_death = tex_death;
        current_weapon = tex_knife;
        is_attacking = false;
        took_damage = false;
        attack_duration = 0.4f; // 400ms for a snappy slash
        attack_timer = 0.0f;
        hurt_duration = 0.2f;
        hurt_timer = 0.0f;
        facing_left = false;
        i_frames = 0.0f;
        hp = 10;
	}

// Update function for moving the player object around
void PlayerGameObject::Update(double delta_time) {
    // Track facing direction based on horizontal movement
    if (velocity.x < -0.1f) facing_left = true;
    else if (velocity.x > 0.1f) facing_left = false;

    // Handle Stun Timer (Input lock for wall bouncing)
    if (stun_timer > 0.0f) {
        stun_timer -= (float)delta_time;
    }

    // Handle Attack Timer
    if (is_attacking) {
        attack_timer -= (float)delta_time;
        if (attack_timer <= 0.0f) {
            is_attacking = false;
        }
    }

    //handle hurt timer
    if (took_damage) {
        hurt_timer -= (float)delta_time;
        if (hurt_timer <= 0.0f) {
            took_damage = false;
        }
    }

    //handle iframes
    if(i_frames > 0.0) {
        i_frames -= (float)delta_time;
    }

    //power-up logic
    speed_timer -= delta_time;

	position_ += velocity * float(delta_time);

    // --- ADVANCED METHOD: ELASTIC COLLISION RESOLUTION (WALLS) ---
    float left_edge = -5.0f;
    float right_edge = 15.0f;

    if (position_.x <= left_edge) {
        position_.x = left_edge; // Resolve penetration
        // Reverse velocity and apply restitution (Energy loss)
        velocity.x = std::abs(velocity.x) + 2.0f * restitution; 
        stun_timer = 0.2f; // Lock input for 200ms
    } 
    else if (position_.x >= right_edge) {
        position_.x = right_edge; // Resolve penetration
        // Reverse velocity and apply restitution
        velocity.x = -std::abs(velocity.x) - 2.0f * restitution;
        stun_timer = 0.2f; // Lock input for 200ms
    }

	if(position_.y > ground || position_.y < ground) {in_air = true;} else {in_air = false;}
	if(in_air) {
		velocity.y -= gravity * (float)delta_time;
		if (position_.y <= ground) {
			position_.y = ground;
			velocity.y = -velocity.y * restitution;
			if(std::abs(velocity.y) <= 0.5) {
				velocity.y = 0.0f;
				in_air = false;
			}
		}
	}
	GameObject::Update(delta_time);
}

//handle weapon switching
void PlayerGameObject::SwitchWeapon() {
    if(has_sword) {
        current_weapon == tex_sword ? current_weapon = tex_knife : current_weapon = tex_sword;
    }
}

void PlayerGameObject::Render(glm::mat4 view_matrix, double current_time) {
    
    int num_frames = 4;
    int current_frame = 0;
    
    // 1. Texture and Frame Logic
    if (is_attacking) {
        texture_ = tex_attack;
        num_frames = 6;
        // Calculate exact frame (0 to 5) based on the timer countdown
        float progress = 1.0f - (attack_timer / attack_duration);
        current_frame = std::clamp((int)(progress * num_frames), 0, num_frames - 1);
    } else if (in_air) {
        texture_ = tex_jump; 
        num_frames = 6;
        current_frame = (int)(current_time * 8.0f) % num_frames;
    } else if (std::abs(velocity.x) > 0.1f) {
        texture_ = tex_run;
        num_frames = 6;
        current_frame = (int)(current_time * 10.0f) % num_frames;
    } else if (hp <= 0) {
        texture_ = tex_death;
        num_frames = 6;
        current_frame = num_frames - 1; 
    } else if (took_damage) {
        texture_ = tex_hurt;
        num_frames = 2;
        current_frame = std::clamp((int)((1.0f - (hurt_timer / hurt_duration)) * num_frames), 0, num_frames - 1);
    } else {
        texture_ = tex_idle;
        num_frames = 4;
        current_frame = (int)(current_time * 4.0f) % num_frames;
    }

    // 2. UV Math
    glm::vec2 uv_scale = glm::vec2(1.0f / (float)num_frames, 1.0f);
    glm::vec2 uv_offset = glm::vec2(current_frame * uv_scale.x, 0.0f);

    // 3. Setup Shaders
    shader_->Enable();
    shader_->SetUniformMat4("view_matrix", view_matrix);
    shader_->SetUniform2f("uv_scale", uv_scale);
    shader_->SetUniform2f("uv_offset", uv_offset);

    // 4. Transform Matrix (with Left-Facing Flip logic)
    float flip_scale = facing_left ? -xscale_ : xscale_;
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(flip_scale, yscale_, 1.0));
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);
    glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;

    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

    // 5. Draw the Player
    geometry_->SetGeometry(shader_->GetShaderProgram());
    glBindTexture(GL_TEXTURE_2D, texture_);
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);

    // 6. LAYERED RENDER: Draw the Knife if attacking
    if (is_attacking) {
        glm::mat4 weapon_matrix = transformation_matrix;
        
        shader_->SetUniformMat4("transformation_matrix", weapon_matrix);
        
        // Bind the knife and draw it using the exact same UV frame!
        glBindTexture(GL_TEXTURE_2D, current_weapon);
        glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
    }
}

void PlayerGameObject::Attack() {
    if (!is_attacking) {
        is_attacking = true;
        attack_timer = attack_duration; // Reset timer for a fresh slash
    }
}

//jump logic
void PlayerGameObject::Jump(void) {
	if(!in_air) {
		velocity = glm::vec3(0.0f, 7.0f, 0.0f);
		in_air = true;
	}
}

} // namespace game
