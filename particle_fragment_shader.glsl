// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;
in float age;

// Texture sampler
uniform sampler2D onetex;

//uniforms
uniform int is_blood;

void main()
{
    if (age > 2.0) {
        discard;
    }

    // Sample texture
    vec4 color = texture2D(onetex, uv_interp);

    float life_factor = 1.0 - (age / 2.0);   
    if(is_blood == 1) {color.rgb = vec3(1.0, 0.0, 0.0) * color_interp.r * life_factor;}
    else {color.rgb = vec3(0.8, 0.4, 0.01) * color_interp.r * life_factor;}
    

    // Assign color to fragment
    gl_FragColor = vec4(color.r, color.g, color.b, color.a);

    // Check for transparency
    if(color.a < 1.0)
    {
         discard;
    }
}
