// Source code of vertex shader for particle system
#version 130

// Vertex buffer
in vec2 vertex; // Vertex coordinates
in vec2 dir; // Velocity
in float t; // Phase
in vec2 uv; // Texture coordinates

// Uniform (global) buffer
uniform mat4 transformation_matrix;
uniform mat4 view_matrix;
uniform float time; // Timer
uniform int is_blood; //to implement 2 different types of particle effects

// Attributes forwarded to the fragment shader
out vec4 color_interp;
out vec2 uv_interp;
out float age;

void main()
{
    vec4 pos; // Vertex position
    float cycle = 1.0; // Duration of cycle in seconds
    float speed = 4.0; // Speed adjustment constant
    float gravity = -5.8; // Gravity in this world
    float acttime; // Cyclic time

    // Add phase to the time and cycle it
    acttime = mod(time + t*cycle, cycle);
    age = acttime;

    if(is_blood == 0) {
        // Move particle along given direction
        pos = vec4(vertex.x + acttime*speed*dir.x , vertex.y + acttime*speed*dir.y , 0.0, 1.0);
    } else {
        // Add gravity
        pos = vec4(vertex.x+dir.x*acttime*speed , vertex.y+dir.y*acttime*speed + 0.5*gravity*acttime*acttime, 0.0, 1.0);
    }

    // Transform vertex position
    gl_Position = view_matrix*transformation_matrix*pos;
    
    // Set color
    //color_interp = vec4(0.5+0.5*cos(4*acttime),0.5*sin(4*acttime)+0.5,0.5, 1.0);
    color_interp = vec4(t, 0.0, 0.0, 1.0);

    // Transfer texture coordinates
    uv_interp = uv;
}
