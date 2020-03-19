#version 460 core
layout (location = 0)in vec3 aPos;
layout (location = 1)in vec4 aColor;
layout (location = 2)in vec2 aTexCoords;


out vec4 vertexColor;
out vec2 texCoords;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
	vertexColor = aColor;
	texCoords = aTexCoords;

	gl_Position = u_ViewProjection * u_Model * vec4(aPos , 1.0);
}
