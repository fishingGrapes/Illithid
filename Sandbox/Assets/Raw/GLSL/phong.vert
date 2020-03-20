#version 460 core
layout (location = 0)in vec3 aPos;
layout (location = 1)in vec4 aColor;
layout (location = 2)in vec2 aTexCoords;
layout (location = 3)in vec3 aNormal;


out vec4 vertexColor;
out vec2 texCoords;
out vec3 normal;
out vec3 fragPosition;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;
uniform mat3 u_NormalMatrix;

void main()
{
	vertexColor = aColor;
	texCoords = aTexCoords;
	normal = u_NormalMatrix * aNormal;
	fragPosition = vec3(u_Model * vec4(aPos, 1.0));

	gl_Position = u_ViewProjection * u_Model * vec4(aPos , 1.0);
}
