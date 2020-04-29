#version 460 core

out vec4 fragColor;
in vec3 texCoords;

uniform samplerCube  u_SkyBox;


void main()
{
	fragColor = texture(u_SkyBox, texCoords);
}

