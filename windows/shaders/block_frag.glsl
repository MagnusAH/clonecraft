#version 450 core

in vec3 normal;
in vec3 toLight;
in vec2 texCoords;

out vec4 outColor;

void main()
{
	float diff = max(dot(normal, toLight), 0.3f);
	outColor = vec4(diff * vec3(0.3f, 0.8f, 0.5f), 1);
}