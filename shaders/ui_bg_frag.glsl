// Shader from:
// https://learnopengl.com/In-Practice/2D-Game/Rendering-Sprites

#version 330 core
in vec2 TexCoords;
out vec4 color;

//uniform sampler2D u_Texture;
//uniform vec3 spriteColor;

void main()
{    
    // color = vec4(spriteColor, 1.0) * texture(u_Texture, TexCoords);
    color = vec4(0.0, 0.278, 0.294, 1.0);
}  