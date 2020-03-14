#version 460 core
layout (location = 0)in vec3 aPos;
layout (location = 1)in vec4 aColor;
layout (location = 2)in vec2 aTexCoords;


out vec4 vertexColor;
out vec2 texCoords;

void main()
{
	vertexColor = aColor;
	texCoords = aTexCoords;

	gl_Position = vec4(aPos * 0.75, 1.0);
}
