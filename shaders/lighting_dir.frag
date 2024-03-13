#version 330 core

struct Material
{
	vec3 ambient;
	sampler2D diffuseMap;
	vec3 specular;
	float shininess;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 frag_color;

uniform DirectionalLight light;
uniform Material material;
uniform vec3 viewPos; 

void main()
{
	// Ambient

	vec3 ambient = light.ambient * material.ambient;

	// Diffuse
	vec3 normal = normalize(Normal); // normalization could be lost due to interpolation of vertices 
	vec3 lightDir = normalize(-light.direction); // negative as it is from the fragment to the light
	float NDotL = max(dot(normal, lightDir), 0.0f); // do not let it go to negative
	vec3 diffuse = light.diffuse * vec3(texture(material.diffuseMap, TexCoord)) * NDotL;
							
									
	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	
	// Specular (Blinn-Phong Model)
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);


	frag_color = vec4(ambient + diffuse + specular, 1.0f); // mix - does a linear interpolation between the colors of two texture witha certain blend factor(0.2).
};
