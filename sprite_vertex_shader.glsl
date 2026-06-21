// Source code of vertex shader
#version 130

// Vertex buffer
in vec2 vertex;
in vec3 color;
in vec2 uv;

// Uniform (global) buffer
uniform mat4 transformation_matrix;
uniform mat4 view_matrix;

// --- UNIFORMS FOR ANIMATION ---
uniform vec2 uv_scale;
uniform vec2 uv_offset;

// Attributes forwarded to the fragment shader
out vec4 color_interp;
out vec2 uv_interp;
out vec2 pos_interp;

void main()
{
    // Transform vertex
    vec4 vertex_pos = vec4(vertex, 0.0, 1.0);
    vec4 transformed_pos = view_matrix * transformation_matrix * vertex_pos;
    gl_Position = transformed_pos;
    pos_interp = transformed_pos.xy;
    
    // Pass attributes to fragment shader
    color_interp = vec4(color, 1.0);
    
    // --- APPLY UV TRANSFORMATIONS ---
    uv_interp = (uv * uv_scale) + uv_offset;
}
