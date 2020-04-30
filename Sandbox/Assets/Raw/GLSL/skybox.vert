#version 460 core
layout (location = 0)in vec3 aPos;

out vec3 texCoords;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	texCoords = aPos;
	vec4 pos = u_Projection  * u_View * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
}
