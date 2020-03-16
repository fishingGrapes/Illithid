#version 460 core
out vec4 fragColor;

in vec4 vertexColor;
in vec2 texCoords;

uniform sampler2D u_BrickWall;
uniform sampler2D u_SmileyFace;

void main()
{
	fragColor = vertexColor * mix( texture(u_BrickWall, texCoords), texture(u_SmileyFace, texCoords), 0.2);
}

