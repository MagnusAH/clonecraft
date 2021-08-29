#version 450 core

uniform vec3 light;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texCoords;

out vec3 normal;
out vec3 toLight;
out vec2 texCoords;

void main()
{
	vec3 lightPos = vec3(128, 128, 128);

	normal = in_normal;
	texCoords = in_texCoords;

	toLight = lightPos - vec3(modelMatrix * vec4(vertex, 1.0));
	toLight = vec3(1, 2, 3); //static light direction
	toLight = normalize(toLight);


	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.0);
}