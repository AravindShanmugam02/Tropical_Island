#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuseTex;
}; 

struct SpotLight {
    vec3 position;
	vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

in Vertex
{
	vec2 texCoord;
	vec3 fragPos;
	vec3 normal;
} IN;

uniform vec3 viewPos;
uniform Material matrl;
uniform SpotLight lit;

void main()
{
	//ambient
    vec3 ambientResult = lit.ambient * texture(matrl.diffuseTex, IN.texCoord).rgb;
	
	//diffuse 
    vec3 norm = normalize(IN.normal);
    vec3 lightDir = normalize(lit.position - IN.fragPos);
    float diffuseImpact = max(dot(norm, lightDir), 0.0);
    vec3 diffuseResult = lit.diffuse * diffuseImpact * texture(matrl.diffuseTex, IN.texCoord).rgb;
	
	vec3 viewDir = normalize(viewPos - IN.fragPos);
	
	//spotlight(soft edges)
    float theta = dot(lightDir, normalize(-viewDir)); 
    float eps = (lit.cutOff - lit.outerCutOff);
    float inten = clamp((theta - lit.outerCutOff) / eps, 0.0, 1.0);
    diffuseResult  *= inten;
	
	//attenuation
    float distance    = length(lit.position - IN.fragPos);
    float attenuation = 1.0 / (lit.constant + lit.linear * distance + lit.quadratic * (distance * distance));    
    ambientResult  *= attenuation; 
    diffuseResult   *= attenuation;
	
	vec3 result = ambientResult + diffuseResult;
    FragColor = vec4(result, 1.0);
} 