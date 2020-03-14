#version 460 core
out vec4 fragColor;

in vec4 vertexColor;
in vec2 texCoords;

uniform sampler2D u_BrickWall;

void main()
{
	fragColor = vertexColor * texture(u_BrickWall, texCoords);
}

