#ifndef GAME_H_
#define GAME_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>

#include "shader.h"
#include "game_object.h"

namespace game {

    class PlayerGameObject;
    class BackgroundGameObject;
    class CollectibleGameObject;
    class PlatformGameObject;
    class EnemyGameObject;
    class DoorGameObject;
    class TextGameObject;

    // A class for holding the main game objects
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();

            // Call Init() before calling any other method
            // Initialize graphics libraries and main window
            void Init(void); 

            // Set up the game world (scene, game objects, etc.)
            void SetupGameWorld(void);

            // Destroy the game world
            void DestroyGameWorld(void);

            // Run the game (keep the game active)
            void MainLoop(void); 

        private:
            // Main window: pointer to the GLFW window structure
            GLFWwindow *window_;

            // Sprite geometry
            Geometry *sprite_;

            //particle geometry
            Geometry *particles;
            Geometry *particles_boss;

            //shader for particle rendering
            Shader particle_shader;

            // Shader for rendering sprites in the scene
            Shader sprite_shader_;

            //text shader
            Shader text_shader_;

            // References to textures
            // This needs to be a pointer
            GLuint *tex_;

            // List of game objects
            std::vector<GameObject*> game_objects_;

            //list of collectibles
            std::vector<CollectibleGameObject*> collectibles;

            //list of platforms
            std::vector<PlatformGameObject*> platforms;

            //list of enemies
            std::vector<EnemyGameObject*> enemies;

            //boss
            EnemyGameObject* boss;

            //player game object
            PlayerGameObject* player;

            //background game object
            BackgroundGameObject* background1;
            BackgroundGameObject* background2;

            //door
            DoorGameObject* door;

            //UI
            TextGameObject* health;
            CollectibleGameObject* key;
            TextGameObject* timer;

            // Keep track of game time
            double current_time_;

            // Camera zoom factor
            float camera_zoom_;

            // Handle user input
            void HandleControls(double delta_time);

            // Update all the game objects
            void Update(double delta_time);
 
            // Render the game world
            void Render(void);

            // Get mouse position in OpenGL coordinates
            // Returns "true" if position could be gathered, "false" otherwise
            // Position is saved to mouse.x and mouse.y
            bool GetMousePosition(glm::vec2 &mouse);

            // Callback for when the window is resized
            static void ResizeCallback(GLFWwindow* window, int width, int height);

            // Set a specific texture
            void SetTexture(GLuint w, const char *fname);

            // Load all textures
            void LoadTextures(std::vector<std::string> &textures);
    }; // class Game

} // namespace game

#endif // GAME_H_
