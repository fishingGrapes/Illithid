#version 460 core
out vec4 fragColor;

in vec4 vertexColor;
in vec2 texCoords;
in vec3 normal;
in vec3 fragPosition;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;

	float shininess;
};

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;

	float innerCutoff;
	float outerCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

#define NR_POINT_LIGHTS 4

uniform Material u_Material;

uniform DirectionalLight u_DirectionalLight;
uniform PointLight u_PointLights[NR_POINT_LIGHTS];
uniform SpotLight u_SpotLight;

uniform vec3 u_ViewPosition;

vec3 CalculateDirectionLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection);

void main()
{
	vec3 n = normalize(normal);
	vec3 viewDirection = normalize(u_ViewPosition - fragPosition);

	//Directional Light
	vec3 result = CalculateDirectionLight(u_DirectionalLight, n, viewDirection);
	//Point Lights
	for(int i=0; i < NR_POINT_LIGHTS; ++i)
	{
		result += CalculatePointLight(u_PointLights[i], n, fragPosition, viewDirection);
	}
	//Spot Light
	result += CalculateSpotLight(u_SpotLight, n, fragPosition, viewDirection);

	fragColor = vertexColor * vec4(result, 1.0);
}

vec3 CalculateDirectionLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
	vec3 inverseLightDirection = normalize(-light.direction);

	//diffusion
	float diffusion = max(dot(normal , inverseLightDirection), 0.0);
	//specularity
	vec3 reflectedDirection = reflect(-inverseLightDirection, normal);
	float specularity = pow(max(dot(viewDirection, reflectedDirection), 0.0), u_Material.shininess );

	vec3 ambientComponent = light.ambient * vec3(texture(u_Material.diffuse, texCoords));
	vec3 diffuseComponent = light.diffuse * diffusion * vec3(texture(u_Material.diffuse, texCoords));
	vec3 specularComponent = light.specular * specularity * vec3(texture(u_Material.specular, texCoords));

	return (ambientComponent + diffuseComponent + specularComponent);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
	vec3 inverseLightDirection = normalize(light.position - fragPosition);

	//diffusion
	float diffusion = max(dot(normal , inverseLightDirection), 0.0);
	//specularity
	vec3 reflectedDirection = reflect(-inverseLightDirection, normal);
	float specularity = pow(max(dot(viewDirection, reflectedDirection), 0.0), u_Material.shininess );
	//attenuation
	float distance = length(light.position - fragPosition);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambientComponent = light.ambient * vec3(texture(u_Material.diffuse, texCoords));
	vec3 diffuseComponent = light.diffuse * diffusion * vec3(texture(u_Material.diffuse, texCoords));
	vec3 specularComponent = light.specular * specularity * vec3(texture(u_Material.specular, texCoords));

	ambientComponent *= attenuation;
	diffuseComponent *= attenuation;
	specularComponent *= attenuation;

	return (ambientComponent + diffuseComponent + specularComponent);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
	vec3 inverseLightDirection = normalize(light.position - fragPosition);

	//diffusion
	float diffusion = max(dot(normal , inverseLightDirection), 0.0);
	//specularity
	vec3 reflectedDirection = reflect(-inverseLightDirection, normal);
	float specularity = pow(max(dot(viewDirection, reflectedDirection), 0.0), u_Material.shininess );
	//attenuation
	float distance = length(light.position - fragPosition);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	//spot intensity
	float theta = dot(inverseLightDirection, normalize(-light.direction));
	float espilon = light.innerCutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / espilon, 0.0, 1.0);

	vec3 ambientComponent = light.ambient * vec3(texture(u_Material.diffuse, texCoords));
	vec3 diffuseComponent = light.diffuse * diffusion * vec3(texture(u_Material.diffuse, texCoords));
	vec3 specularComponent = light.specular * specularity * vec3(texture(u_Material.specular, texCoords));

	ambientComponent *= attenuation * intensity;
	diffuseComponent *= attenuation * intensity;
	specularComponent *= attenuation * intensity;

	return (ambientComponent + diffuseComponent + specularComponent);
}
