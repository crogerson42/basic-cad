#version 410 core

// From vertex shader
in vec3 v_vertexColors;
in vec3 v_vertexNormals;
in vec3 v_worldSpaceFragment;

// Uniform light variables
uniform vec3 u_viewPosition;

out vec4 FragColor;

void main() {
    vec3 u_LightPos = u_viewPosition;              // assume camera casts light (for now)
    vec3 u_LightColor = vec3(1.0, 1.0, 1.0);
    vec3 u_AmbientColor = vec3(1.0, 1.0, 1.0);     // Ambient color
    vec3 u_DiffuseColor = vec3(1.0, 1.0, 1.0);     // Diffuse color
    vec3 u_SpecularColor = vec3(1.0, 1.0, 1.0);    // Specular color
    float u_Shininess = 0.95;                      // Shininess factor for specular highlights

    vec3 normals = normalize(v_vertexNormals);
    vec3 lightDirection = normalize(u_LightPos - v_worldSpaceFragment);

    // Ambient component: a base level of lighting applied to all fragments
    vec3 ambient = u_AmbientColor * u_LightColor;

    // Diffuse component: depends on angle between light direction and surface normal
    float diff = max(0.0, dot(normals, lightDirection));
    vec3 diffuse = diff * u_DiffuseColor * u_LightColor;

    // Specular component: creates shiny highlights using Phong reflection model
    vec3 viewDir = normalize(u_viewPosition - v_worldSpaceFragment); // Direction from fragment to viewer
    vec3 reflectDir = reflect(-lightDirection, normals);    // Direction of the reflected light ray
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);
    vec3 specular = spec * u_SpecularColor * u_LightColor;

	// calculate attenuation
	float distance = length(u_LightPos - v_worldSpaceFragment);
	// float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    float attenuation = 1.0;
    // / (constant + linear * distance + quadratic * (distance * distance));

	// apply attenuation
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

    // Combine the components to produce the final color
    vec3 lighting = ambient + diffuse + specular * 0;

    vec3 lit_object = v_vertexColors * lighting;

	FragColor = vec4(lit_object, 1.0);








    // FragColor = vec4(fragColor, 1.0f);
}