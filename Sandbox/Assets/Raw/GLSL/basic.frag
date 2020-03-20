#version 460 core
out vec4 fragColor;

in vec4 vertexColor;
in vec2 texCoords;

uniform sampler2D u_BrickWall;
uniform vec4 u_Color;


void main()
{
	fragColor = vertexColor * u_Color;
}

