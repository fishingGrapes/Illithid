#version 460 core
out vec4 fragColor;

in vec4 vertexColor;
in vec2 texCoords;
in vec3 normal;
in vec3 fragPosition;

uniform sampler2D u_BrickWall;
uniform vec4 u_Color;

uniform float u_AmbientStrength;
uniform vec4 u_LightColor;
uniform vec3 u_LightPosition;

void main()
{
	vec3 ambience = (u_AmbientStrength * u_LightColor.rgb);

	vec3 n = normalize(normal);
	vec3 lightDirection = normalize(u_LightPosition - fragPosition);

	float diffuse = max(dot(n , lightDirection), 0.0);
	vec3 diffuseComponent = diffuse * u_LightColor.rgb;

	fragColor = vertexColor * u_Color * vec4((ambience + diffuseComponent), 1.0);
}

