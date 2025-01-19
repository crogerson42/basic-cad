// Shader from:
// https://learnopengl.com/In-Practice/2D-Game/Rendering-Sprites

#version 330 core
// layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 u_ModelMatrix;
uniform mat4 u_Projection;

void main()
{
    //TexCoords = vertex.zw;
    //gl_Position = u_Projection * u_ModelMatrix * vec4(vertex.xy, 0.0, 1.0);

    TexCoords = texCoords;
    gl_Position = u_Projection * u_ModelMatrix * vec4(position, 0.0, 1.0);
}