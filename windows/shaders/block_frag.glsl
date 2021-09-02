#version 450 core

in vec3 normal;
in vec3 toLight;
in vec2 texCoords;

out vec4 outColor;

uniform sampler2D atlas;

void main()
{
	float diffuse = max(dot(normal, toLight), 0.3f);
	outColor = diffuse * texture(atlas, texCoords);
}