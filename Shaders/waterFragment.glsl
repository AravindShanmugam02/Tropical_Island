#version 330 core

uniform sampler2D diffuseTex;
uniform samplerCube cubeTex;

uniform vec3 cameraPos;

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 fragColour;

void main(void)
{
	vec4 diffuse = texture(diffuseTex, IN.texCoord);
	diffuse.w = 0.05f;
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 reflectDir = reflect(-viewDir, normalize(IN.normal));
	vec4 reflectTex = texture(cubeTex, reflectDir);
	
	fragColour = (reflectTex * 0.25f) + (diffuse); //* 0.25f
}