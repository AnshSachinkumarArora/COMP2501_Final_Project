#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include "shader.h"
#include "geometry.h"

namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of one object in the game world
        The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)
    */
    class GameObject {

        public:
            // Constructor
            GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            // Update the GameObject's state. Can be overriden in children
            virtual void Update(double delta_time);

            // Renders the GameObject. Can also be overriden in children
            virtual void Render(glm::mat4 view_matrix, double current_time);

            // Getters
            //
            // Get position in OpenGL coordinates
            inline glm::vec3 GetPosition(void) const { return position_; }
            // Get scale (size) of the object
            inline float GetScale(void) const { return xscale_; }
            inline float GetXScale(void) const { return xscale_; }
            inline float GetYScale(void) const { return yscale_; }
            // Get current orientation
            inline float GetRotation(void) const { return angle_; }

            // Get bearing direction (vector pointing to the direction in which
            // the game object is facing)
            glm::vec3 GetBearing(void) const;

            // Get vector pointing to the right side of the game object
            glm::vec3 GetRight(void) const;

            // Setters
            inline void SetPosition(const glm::vec3& position) { position_ = position; }
            inline void SetScale(float scale) { xscale_ = scale; yscale_ = scale; }
            inline void SetScale(float xscale, float yscale) { xscale_ = xscale; yscale_ = yscale; }
            void SetRotation(float angle);


        protected:
            // Object's Transform Variables
            glm::vec3 position_;
            float xscale_;
            float yscale_;
            float angle_;

            // Geometry
            Geometry *geometry_;
 
            // Shader
            Shader *shader_;

            // Object's texture reference
            GLuint texture_;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
