#version 460 core

out vec4 fragColor;
in vec4 vertexColor;

uniform vec4 u_Color;


void main()
{
	fragColor = u_Color;
}

