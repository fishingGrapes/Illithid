#version 460 core
out vec4 fragColor;

in vec4 vertexColor;
in vec2 texCoords;
in vec3 normal;
in vec3 fragPosition;

subroutine vec3 lighting_t(vec3 normal, vec3 viewDirection);

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


uniform Material u_Material;

uniform DirectionalLight u_DirectionalLight;
uniform PointLight u_PointLight;
uniform SpotLight u_SpotLight;

uniform vec3 u_ViewPosition;
uniform int u_LightingMethod;

subroutine uniform lighting_t u_LightingRoutine;

vec3 CalculateDirectionLight( vec3 normal, vec3 viewDirection);
vec3 CalculatePointLight( vec3 normal, vec3 viewDirection);
vec3 CalculateSpotLight( vec3 normal, vec3 viewDirection);

void main()
{
	vec3 n = normalize(normal);
	vec3 viewDirection = normalize(u_ViewPosition - fragPosition);

	vec3 result = u_LightingRoutine(n, viewDirection);
	fragColor = vec4(result, 1.0);
}


subroutine(lighting_t)
vec3 CalculateDirectionLight(vec3 normal, vec3 viewDirection)
{
	vec3 inverseLightDirection = normalize(-u_DirectionalLight.direction);

	//diffusion
	float diffusion = max(dot(normal , inverseLightDirection), 0.0);
	//specularity
    vec3 halfwayDirection = normalize(inverseLightDirection + viewDirection);  
	float specularity = pow(max(dot(normal, halfwayDirection), 0.0), u_Material.shininess );

	vec3 ambientComponent = u_DirectionalLight.ambient * vec3(texture(u_Material.diffuse, texCoords));
	vec3 diffuseComponent = u_DirectionalLight.diffuse * diffusion * vec3(texture(u_Material.diffuse, texCoords));
	vec3 specularComponent = u_DirectionalLight.specular * specularity * vec3(texture(u_Material.specular, texCoords));

	return (ambientComponent + diffuseComponent + specularComponent);
}

subroutine(lighting_t)
vec3 CalculatePointLight(vec3 normal, vec3 viewDirection)
{
	vec3 inverseLightDirection = normalize(u_PointLight.position - fragPosition);

	//diffusion
	float diffusion = max(dot(normal , inverseLightDirection), 0.0);
	//specularity
    vec3 halfwayDirection = normalize(inverseLightDirection + viewDirection);  
	float specularity = pow(max(dot(normal, halfwayDirection), 0.0), u_Material.shininess );
	//attenuation
	float distance = length(u_PointLight.position - fragPosition);
	float attenuation = 1.0 / (u_PointLight.constant + u_PointLight.linear * distance + u_PointLight.quadratic * (distance * distance));

	vec3 ambientComponent = u_PointLight.ambient * vec3(texture(u_Material.diffuse, texCoords));
	vec3 diffuseComponent = u_PointLight.diffuse * diffusion * vec3(texture(u_Material.diffuse, texCoords));
	vec3 specularComponent = u_PointLight.specular * specularity * vec3(texture(u_Material.specular, texCoords));

	ambientComponent *= attenuation;
	diffuseComponent *= attenuation;
	specularComponent *= attenuation;

	return (ambientComponent + diffuseComponent + specularComponent);
}

subroutine(lighting_t)
vec3 CalculateSpotLight(vec3 normal, vec3 viewDirection)
{
	vec3 inverseLightDirection = normalize(u_SpotLight.position - fragPosition);

	//diffusion
	float diffusion = max(dot(normal , inverseLightDirection), 0.0);
	//specularity
	vec3 halfwayDirection = normalize(inverseLightDirection + viewDirection);  
	float specularity = pow(max(dot(normal, halfwayDirection), 0.0), u_Material.shininess );
	//attenuation
	float distance = length(u_SpotLight.position - fragPosition);
	float attenuation = 1.0 / (u_SpotLight.constant + u_SpotLight.linear * distance + u_SpotLight.quadratic * (distance * distance));
	//spot intensity
	float theta = dot(inverseLightDirection, normalize(-u_SpotLight.direction));
	float espilon = u_SpotLight.innerCutoff - u_SpotLight.outerCutoff;
	float intensity = clamp((theta - u_SpotLight.outerCutoff) / espilon, 0.0, 1.0);

	vec3 ambientComponent = u_SpotLight.ambient * vec3(texture(u_Material.diffuse, texCoords));
	vec3 diffuseComponent = u_SpotLight.diffuse * diffusion * vec3(texture(u_Material.diffuse, texCoords));
	vec3 specularComponent = u_SpotLight.specular * specularity * vec3(texture(u_Material.specular, texCoords));

	ambientComponent *= attenuation * intensity;
	diffuseComponent *= attenuation * intensity;
	specularComponent *= attenuation * intensity;

	return (ambientComponent + diffuseComponent + specularComponent);
}
