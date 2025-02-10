#version 330 core
out vec4 fragColour;

in Vertex{
	vec2 texCoord;
	vec4 fragPos;
	vec3 normal;
} IN;

struct Material{
sampler2D diffuseTex;
};

struct DirectionalLight{
vec3 direction;
vec3 ambient;
vec3 diffuse;
};

uniform vec3 viewPos;
uniform Material matrl;
uniform DirectionalLight lit;

void main(void)
{
	//ambient
	vec3 ambient = lit.ambient * texture(matrl.diffuseTex, IN.texCoord).rgb;
	
	//diffuse
	vec3 norm = normalize(IN.normal);
	vec3 lightDir = normalize(-lit.direction);
	float diffuseImpact = max(dot(norm, lightDir), 0.0);
	vec3 diffuseResult = lit.diffuse * diffuseImpact * texture(matrl.diffuseTex, IN.texCoord).rgb;
	
	vec3 result = ambient + diffuseResult;
	fragColour = vec4(result, 1.0);
}