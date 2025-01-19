#version 410 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;

out vec3 fragColor;

uniform mat4 u_ModelMatrix; // Object space
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;

void main() {
    gl_Position = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0f);
    fragColor = color;
}
