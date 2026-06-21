#include <stdexcept>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <iostream>
#include <algorithm>
#include <queue>

#include <path_config.h>

#include "sprite.h"
#include "shader.h"
#include "player_game_object.h"
#include "background_game_object.h"
#include "game.h"
#include "collectible_game_object.h"
#include "platform_game_object.h"
#include "enemy_game_object.h"
#include "door_game_object.h"
#include "text_game_object.h"
#include "particles.h"
#include "particle_system.h"

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "Game Demo";
const unsigned int window_width_g = 1000;
const unsigned int window_height_g = 800;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

// Directory with game resources such as textures
const std::string resources_directory_g = RESOURCES_DIRECTORY;


void Game::SetupGameWorld(void)
{

    // Setup the game world

    // **** Load all the textures that we will need
    //
    // Declare all the textures here
    std::vector<std::string> textures;
    // Add the textures in the same order as the enum above
    textures.push_back("/textures/chopper.png"); 
    textures.push_back("/textures/ufo.png");
    textures.push_back("/textures/orb.png");
    textures.push_back("/textures/background1.png");
    textures.push_back("/textures/background2.png");
    textures.push_back("/textures/playerDoubleJump.png");
    textures.push_back("/textures/playerIdle.png");
    textures.push_back("/textures/playerRun.png");
    textures.push_back("/textures/playerAttack.png");
    textures.push_back("/textures/knife.png");
    textures.push_back("/textures/sword.png"); //10
    textures.push_back("/textures/swordGround.png");
    textures.push_back("/textures/key.png");
    textures.push_back("/textures/powerup.png");
    textures.push_back("/textures/platformTile.png");
    textures.push_back("/textures/enemyAttack.png"); //15
    textures.push_back("/textures/enemyDeath.png");
    textures.push_back("/textures/enemyHurt.png");
    textures.push_back("/textures/enemyIdle.png");
    textures.push_back("/textures/enemyWalk.png");
    textures.push_back("/textures/playerHurt.png"); //20
    textures.push_back("/textures/playerDeath.png");
    textures.push_back("/textures/doorClosed.png");
    textures.push_back("/textures/doorOpen.png");
    textures.push_back("/textures/Attack.png");
    textures.push_back("/textures/Death.png"); //25
    textures.push_back("/textures/Hurt.png"); //26
    textures.push_back("/textures/Idle.png");
    textures.push_back("/textures/Walk.png");
    textures.push_back("/textures/font.png");
    textures.push_back("/textures/bossAttack.png"); //30
    textures.push_back("/textures/bossDeath.png"); 
    textures.push_back("/textures/bossHurt.png"); 
    textures.push_back("/textures/bossIdle.png");
    textures.push_back("/textures/bossWalk.png");
    // Load all the textures
    LoadTextures(textures);


    // **** Setup all the game objects in the world

    // Setup the player object (position, geometry, shader, texture)

    // Create the player object
    player = new PlayerGameObject(glm::vec3(-4.0f, -4.0f, 0.0f), sprite_, &sprite_shader_, tex_[0], tex_[5], tex_[6], tex_[7], tex_[8], tex_[9], tex_[10], tex_[20], tex_[21]);
    // Make the object larger
    player->SetScale(2.0); 

    //create door
    door = new DoorGameObject(glm::vec3(14.0f, -4.5f, 0.0f), sprite_, &sprite_shader_, tex_[22], tex_[23]);
    door->SetScale(1.0);

    //setup ui
    health = new TextGameObject(glm::vec3(0.0f, -3.3f, 0.0f), sprite_, &text_shader_, tex_[29]);
    key = new CollectibleGameObject(glm::vec3(0.0f, -3.3f, 0.0f), sprite_, &sprite_shader_, tex_[12], TYPE_KEY);
    timer = new TextGameObject(glm::vec3(0.0f, -3.3f, 0.0f), sprite_, &text_shader_, tex_[29]);

    //create collectibles
    CollectibleGameObject* sword = new CollectibleGameObject(glm::vec3(3.0f, -2.3f, 0.0f), sprite_, &sprite_shader_, tex_[11], TYPE_SWORD);
    CollectibleGameObject* list_key = new CollectibleGameObject(glm::vec3(12.0f, -4.2f, 0.0f), sprite_, &sprite_shader_, tex_[12], TYPE_KEY);
    CollectibleGameObject* speed = new CollectibleGameObject(glm::vec3(-3.0f, -1.3f, 0.0f), sprite_, &sprite_shader_, tex_[13], TYPE_SPEED_BOOST);
    sword->SetScale(0.75);
    key->SetScale(0.75);
    speed->SetScale(0.75);
    collectibles.push_back(sword);
    collectibles.push_back(list_key);
    collectibles.push_back(speed);

    //create enemies
    for(int i = 0; i < 2; i++) {
        EnemyGameObject* e1;
        if(i%2 == 0) {
            e1 = new EnemyGameObject(glm::vec3(3.0f, -2.0f, 0.0f), sprite_, &sprite_shader_, tex_[18], tex_[19], tex_[15], tex_[17], tex_[16], player, MOB);
        } else {
            e1 = new EnemyGameObject(glm::vec3(-2.8f, -1.0f, 0.0f), sprite_, &sprite_shader_, tex_[27], tex_[28], tex_[24], tex_[26], tex_[25], player, TANK);
        }
        e1->SetScale(2.0);
        enemies.push_back(e1);
    }
    boss = new EnemyGameObject(glm::vec3(13.0f, -4.0f, 0.0f), sprite_, &sprite_shader_, tex_[33], tex_[34], tex_[30], tex_[32], tex_[31], player, BOSS);
    boss->SetScale(4.0f);
    enemies.push_back(boss);

    //create platforms
    float start = 2.5f;
    for(int i = 0; i < 5; i++) {
        PlatformGameObject* p1 = new PlatformGameObject(glm::vec3(start, -2.5f, 0.0f), sprite_, &sprite_shader_, tex_[14]);
        start += p1->GetScale()-0.5f;
        p1->SetScale(1.0);
        platforms.push_back(p1);
    }
    start = -3.5f;
    for(int i = 0; i < 8; i++) {
        PlatformGameObject* p1 = new PlatformGameObject(glm::vec3(start, -1.5f, 0.0f), sprite_, &sprite_shader_, tex_[14]);
        start += p1->GetScale()-0.5f;
        p1->SetScale(1.0);
        platforms.push_back(p1);
    }

    // define standard scale for backgrounds
    float bg_scale = 10.0f; 

    // 1. Setup the Industrial Background (Centered at 0)
    // Because the base sprite is 1x1 (from -0.5 to +0.5)
    background1 = new BackgroundGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[3]);
    background1->SetScale(bg_scale);

    // 2. Setup the Cyberpunk Background
    // To make it touch the right edge of the industrial background perfectly,
    // its center needs to be offset by exactly the width of the scale.
    background2 = new BackgroundGameObject(glm::vec3(bg_scale, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[4]);
    background2->SetScale(bg_scale);
}


void Game::DestroyGameWorld(void)
{
    // Free memory for all objects
    // Only need to delete objects that are not automatically freed
    for (int i = 0; i < game_objects_.size(); i++){
        delete game_objects_[i];
    }
}


void Game::HandleControls(double delta_time)
{
    // Get current position and angle
    glm::vec3 curpos = player->GetPosition();
    float angle = player->GetRotation();
    // Compute current bearing direction
    glm::vec3 dir = player->GetBearing();
    // Adjust motion increment and angle increment 
    // if translation or rotation is too slow
    float speed = delta_time*500.0;
    float angle_increment = (glm::pi<float>() / 1800.0f)*speed;
    glm::vec3 current_velocity = player->GetVelocity();
    float move_speed;
    player->SpeedTimer() > 0.0 ? move_speed = 8.0f : move_speed = 2.5f;
    // --- ONLY ACCEPT HORIZONTAL INPUT IF NOT STUNNED ---
    if (!player->IsStunned()) {
        current_velocity.x = 0.0f; // Reset

        if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
            current_velocity.x = move_speed;
        }
        if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
            current_velocity.x = -move_speed;
        }
    }

    player->SetVelocity(current_velocity);

    //jumping logic
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        player->Jump();
    }
    if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS) {
        player->SwitchWeapon();
    }
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }
    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
        player->Attack();
    }

}


void Game::Update(double delta_time)
{
    //check if dead
    if(player->isDead()) {
        // Print HUD stats to terminal
        std::cout << "\n=====================================" << std::endl;
        std::cout << " MISSION FAILED! YOU DIED" << std::endl;
        std::cout << " FINAL SURVIVAL TIME: " << current_time_ << " SECONDS" << std::endl;
        std::cout << "=====================================\n" << std::endl;

        //end game
        glfwSetWindowShouldClose(window_, true);
    }

    // player/enemy platforming/combat interaction logic
    for(int j = 0; j <= enemies.size(); j++){
        // Floor logic
        float default_ground = -4.0f; 
        float current_highest_floor = default_ground; 
        float p_left, p_right, p_bottom; 
        EnemyGameObject* enemy = nullptr;
        if(j < enemies.size()) {
            enemy = enemies[j];
            p_left = enemy->GetPosition().x - (enemy->GetScale() / 4.0f); 
            p_right = enemy->GetPosition().x + (enemy->GetScale() / 4.0f);
            p_bottom = enemy->GetPosition().y;
        } else {
            p_left = player->GetPosition().x - (player->GetScale() / 4.0f); 
            p_right = player->GetPosition().x + (player->GetScale() / 4.0f);
            p_bottom = player->GetPosition().y; 
        } 
        for (int i = 0; i < platforms.size(); i++) {
            PlatformGameObject* plat = platforms[i];
            
            float plat_left = plat->GetPosition().x - (plat->GetScale() / 2.0f);
            float plat_right = plat->GetPosition().x + (plat->GetScale() / 2.0f);
            float plat_top = plat->GetPosition().y + (plat->GetScale() / 2.0f);

            if (p_right > plat_left && p_left < plat_right) {
                if (p_bottom >= plat_top - 0.1f) {
                    if (plat_top > current_highest_floor) {
                        current_highest_floor = plat_top + 0.25f; 
                    }
                }
            }
        }

        //combat logic
        if(j < enemies.size()) {
            enemy = enemies[j];
            glm::vec3 player_center = player->GetPosition();
            glm::vec3 enemy_center = enemy->GetPosition();
            float player_radius = player->GetScale()/2.0;
            float enemy_radius = enemy->GetScale()/2.0;
            float dist = glm::distance(player_center, enemy_center);
            float combined_radius = player_radius + enemy_radius;
            if(dist <= combined_radius-0.5f && !enemy->IsDead()) {
                if(player->isAttacking()) {
                    if (enemy->GetType() != BOSS) {
                        ParticleSystem *particles_ = new ParticleSystem(glm::vec3(-0.5f, -0.5f, 0.0f), particles, &particle_shader, tex_[2], enemy, true);
                        particles_->SetScale(0.2f);
                        enemy->TakeDamage(player->DealDamage(), particles_);
                    } else {
                        ParticleSystem *particles_ = new ParticleSystem(glm::vec3(0.5f, -0.5f, 0.0f), particles_boss, &particle_shader, tex_[2], enemy, false);
                        particles_->SetScale(0.2f);
                        enemy->TakeDamage(player->DealDamage(), particles_);
                    }
                }
                if (enemy->GetState() == ENEMY_ATTACK) {
                    player->TakeDamage(enemy->DealDamage());
                    //std::cout<<player->GetHealth()<<std::endl;
                }
            }
        }

        // Apply updates
        if (j < enemies.size()) { 
            enemy->SetGround(current_highest_floor);
            enemy->Update(delta_time);
        } else {
            player->SetGround(current_highest_floor);
            player->Update(delta_time);
        }
    }
    

    //update all collectibles
    for(int i = collectibles.size()-1; i >= 0; i--) {
        CollectibleGameObject* collectible = collectibles[i];
        collectible->Update(delta_time);
        //collision detection with collectibles
        glm::vec3 player_center = player->GetPosition();
        glm::vec3 collectible_center = collectible->GetPosition();
        float player_radius = player->GetScale()/2.0;
        float collectible_radius = collectible->GetScale()/2.0;
        float dist = glm::distance(player_center, collectible_center);
        float combined_radius = player_radius + collectible_radius;
        if(dist <= combined_radius-0.5f) {
            CollectibleType type = collectible->GetType();
            // grant applicable effects
            if(type == TYPE_SWORD) { player->UnlockSword(); }
            else if(type == TYPE_KEY) { player->GiveKey(); }
            else if(type == TYPE_SPEED_BOOST) { player->ActivateSpeedBoost(); }
            delete collectible;
            collectibles.erase(collectibles.begin() + i);
        }
    }

    //update door
    glm::vec3 player_center = player->GetPosition();
    glm::vec3 door_center = door->GetPosition();
    float player_radius = player->GetScale()/2.0;
    float door_radius = door->GetScale()/2.0;
    float dist = glm::distance(player_center, door_center);
    float combined_radius = player_radius + door_radius;
    if(dist <= combined_radius-0.5f && player->HasKey() && !door->IsOpen() && boss->IsDead()) {
        door->SetDoorOpen();
        // Print HUD stats to terminal
        std::cout << "\n=====================================" << std::endl;
        std::cout << " MISSION ACCOMPLISHED! ESCAPE SUCCESS" << std::endl;
        std::cout << " FINAL SURVIVAL TIME: " << current_time_ << " SECONDS" << std::endl;
        std::cout << "=====================================\n" << std::endl;
        
        // End the game
        glfwSetWindowShouldClose(window_, true);
    }
    door->Update(delta_time);

    // Update the HUD text dynamically
    health->SetText("HP: " + std::to_string(player->GetHealth()));
    timer->SetText("Timer: " + std::to_string(current_time_));

    // Update current game time
    current_time_ += delta_time;
}


void Game::Render(void){

    // Clear background
    glClearColor(viewport_background_color_g.r,
                 viewport_background_color_g.g,
                 viewport_background_color_g.b, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use aspect ratio to properly scale the window
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    glm::mat4 window_scale_matrix(1.0f);
    float aspect_ratio = 1.0f; // Store this to calculate camera width
    
    if (width > height){
        aspect_ratio = ((float) width)/((float) height);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/aspect_ratio, 1.0f, 1.0f));
    } else {
        aspect_ratio = ((float) height)/((float) width);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f/aspect_ratio, 1.0f));
    }

    // Set view to zoom out
    glm::mat4 camera_zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom_, camera_zoom_, camera_zoom_));

    // Calculate how much world space the camera can see based on zoom and aspect ratio
    float scale_x = (width > height) ? (camera_zoom_ / aspect_ratio) : camera_zoom_;
    float camera_half_width = 1.0f / scale_x;

    // Camera clamping logic

    // Define world boundaries based on two backgrounds
    float bg_scale = 10.0f; 
    
    // Left edge of the world (Left edge of Industrial bg)
    float min_world_x = 0.0f - (bg_scale / 2.0f); 
    
    // Right edge of the world (Right edge of Cyberpunk bg)
    float max_world_x = bg_scale + (bg_scale / 2.0f); 

    // The camera's center cannot go past these points without exposing the blue void
    float min_camera_x = min_world_x + camera_half_width;
    float max_camera_x = max_world_x - camera_half_width;

    // Failsafe: If the user stretches the window wider than the entire game world,
    // lock the camera to the center of the world to avoid visual glitching.
    if (min_camera_x > max_camera_x) {
        min_camera_x = (min_world_x + max_world_x) / 2.0f;
        max_camera_x = min_camera_x;
    }

    // Get the player's X position
    float player_x = player->GetPosition().x;

    // Clamp the camera's X position using std::clamp 
    float camera_x = std::clamp(player_x, min_camera_x, max_camera_x);

    // Lock the Y position to make it a strict X-axis side-scroller
    float camera_y = -1.0f; 

    // Create the camera translation matrix (moving the world opposite to the camera)
    glm::mat4 camera_translation = glm::translate(glm::mat4(1.0f), glm::vec3(-camera_x, -camera_y, 0.0f));

    // Combine matrices
    glm::mat4 view_matrix = window_scale_matrix * camera_zoom_matrix * camera_translation;
    
    // Render the Text Object
    health->SetPosition(glm::vec3(-0.9f, 0.9f, 0.0f)); 
    health->SetScale(0.35f, 0.1f); 
    health->Render(window_scale_matrix, current_time_);
    timer->SetPosition(glm::vec3(0.0f, 0.9f, 0.0f));
    timer->SetScale(0.5f, 0.1f); 
    timer->Render(window_scale_matrix, current_time_);

    //render key in hud if player has the key
    if(player->HasKey()) {
        key->SetPosition(glm::vec3(0.95, 0.85, 0.0f));
        key->SetScale(0.15);
        key->Render(window_scale_matrix, current_time_);
    }

    //render player
    player->Render(view_matrix, current_time_);
    //render all enemies and particles
    std::queue<ParticleSystem*> particle_systems;
    for(int i = 0; i < enemies.size(); i++) {
        if(enemies[i]->GetParticles() != nullptr && !enemies[i]->FinishedExploding()) {
            particle_systems.push(enemies[i]->GetParticles());
        }
        enemies[i]->Render(view_matrix, current_time_);
    }
    // Render all collectibles
    for (int i = 0; i < collectibles.size(); i++) {
        collectibles[i]->Render(view_matrix, current_time_);
    }
    //render all platforms
    for (int i = 0; i < platforms.size(); i++) {
        platforms[i]->Render(view_matrix, current_time_);
    }
    //render door
    door->Render(view_matrix, current_time_);
    //render backgrounds
    background1->Render(view_matrix, current_time_);
    background2->Render(view_matrix, current_time_);
    while(!particle_systems.empty()) {
        ParticleSystem *p = particle_systems.front();
        particle_systems.pop();
        p->Render(view_matrix, current_time_);
    }
    
}


void Game::MainLoop(void)
{
    // Loop while the user did not close the window
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window_)){

        // Calculate delta time
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;

        // Update window events like input handling
        glfwPollEvents();

        // Handle user input
        HandleControls(delta_time);

        // Update all the game objects
        Update(delta_time);

        // Render all the game objects
        Render();

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);
    }
}


bool Game::GetMousePosition(glm::vec2 &mouse){

    // Get mouse pixel position in the window
    double x, y;
    glfwGetCursorPos(window_, &x, &y);

    // Get dimensions of the window
    int width, height;
    glfwGetWindowSize(window_, &width, &height);

    // If the mouse is outside the window, return false
    if (x < 0 || x > width || y < 0 || y > height) {
        return false;
    }

    // Get position in world coordinates corresponding to the mouse
    // pixel position on the window
    if (width > height){
        float aspect_ratio = ((float) width)/((float) height);
        mouse.x = ((2.0f*x - width)*aspect_ratio)/(width*camera_zoom_);
        mouse.y = (-2.0f*y + height)/(height*camera_zoom_);
    } else {
        float aspect_ratio = height/width;
        mouse.x = (2.0f*x - width)/(width*camera_zoom_);
        mouse.y = ((-2.0f*y + height)*aspect_ratio)/(height*camera_zoom_);
    }

    return true;
}


Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void)
{

    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    // Set whether window can be resized
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
    if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }

    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Initialize sprite geometry
    sprite_ = new Sprite();
    sprite_->CreateGeometry();

    // set up particle geometry
    Particles *particles_temp = new Particles();
    particles_temp->CreateGeometry(4000); // Use 4000 particles
    particles = particles_temp;

    Particles *particles_temp_boss = new Particles();
    particles_temp_boss->CreateGeometry(1000); // Use 4000 particles
    particles_boss = particles_temp_boss;

    // Initialize sprite shader
    sprite_shader_.Init((resources_directory_g+std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/sprite_fragment_shader.glsl")).c_str());

    //init text shader
    text_shader_.Init((resources_directory_g+std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/text_fragment_shader.glsl")).c_str());

    // Initialize particle shader
    particle_shader.Init((resources_directory_g+std::string("/particle_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/particle_fragment_shader.glsl")).c_str());

    // Initialize time
    current_time_ = 0.0;

    // Set zoom factor
    camera_zoom_ = 0.25f;
}


Game::~Game()
{
    // Free memory used by game world
    DestroyGameWorld();

    // Free rendering resources
    delete sprite_;
    delete particles;

    // Close window
    glfwDestroyWindow(window_);
    glfwTerminate();
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
}


void Game::SetTexture(GLuint w, const char *fname)
{
    // Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, w);

    // Load texture from a file to the buffer
    int width, height;
    unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image){
        std::cout << "Cannot load texture " << fname << std::endl;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    // Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Game::LoadTextures(std::vector<std::string> &textures)
{
    // Allocate a buffer for all texture references
    int num_textures = textures.size();
    tex_ = new GLuint[num_textures];
    glGenTextures(num_textures, tex_);
    // Load each texture
    for (int i = 0; i < num_textures; i++){
        SetTexture(tex_[i], (resources_directory_g+textures[i]).c_str());
    }
    // Set first texture in the array as default
    glBindTexture(GL_TEXTURE_2D, tex_[0]);
}

} // namespace game
