#version 460 core
out vec4 fragColor;

in vec4 vertexColor;
in vec2 texCoords;
in vec3 normal;
in vec3 fragPosition;

uniform sampler2D u_BrickWall;
uniform vec4 u_Color;

uniform float u_AmbientStrength;
uniform float u_SpecularStrength;

uniform vec4 u_LightColor;

uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;

void main()
{
	vec3 ambience = (u_AmbientStrength * u_LightColor.rgb);

	vec3 n = normalize(normal);
	vec3 lightDirection = normalize(u_LightPosition - fragPosition);

	float diffuse = max(dot(n , lightDirection), 0.0);
	vec3 diffuseComponent = diffuse * u_LightColor.rgb;

	vec3 viewDirection = normalize(u_ViewPosition - fragPosition);
	vec3 reflectedDirection = reflect(-lightDirection, n);

	float specular = pow(max(dot(viewDirection, reflectedDirection), 0.0), 32 );
	vec3 specularComponent = u_SpecularStrength * specular * u_LightColor.rgb;

	fragColor = vertexColor * u_Color * vec4((ambience + diffuseComponent + specularComponent), 1.0);
}

