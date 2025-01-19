#version 410 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;

// Uniform variables
uniform mat4 u_ModelMatrix; // Object space
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;

// Uniform light variables
uniform vec3 u_viewPosition;

// Pass to fragment shader
out vec3 v_vertexColors;
out vec3 v_vertexNormals;
out vec3 v_worldSpaceFragment;

void main() {
    v_vertexColors = color;
    v_vertexNormals = normal;

    v_worldSpaceFragment = vec3(u_ModelMatrix * vec4(position, 1.0f));

    gl_Position = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0f);
}
