#version 330 core

uniform sampler2D diffuseTex1; //0
uniform sampler2D bumpTex1; //1

uniform sampler2D diffuseTex2; //2
uniform sampler2D bumpTex2; //3

uniform sampler2D diffuseTex3; //4
uniform sampler2D bumpTex3; //5

uniform sampler2D diffuseTex4; //6
uniform sampler2D bumpTex4; //7

uniform sampler2D blendMap; //8


uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec4 lightColour;
uniform float lightRadius;

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
} IN;

out vec4 fragColour;

void main(void)
{
	vec3 incident = normalize(lightPos - IN.worldPos);
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);
	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));
	
	vec2 scaledTexCoord = vec2(IN.texCoord.x, IN.texCoord.y); //didn't scale here, instead scaled in c++ code snippet.
	vec4 blendMapColour = texture(blendMap, scaledTexCoord); //gets rgb values from the blendmap for each triangle primitive.
	float blackColourAmount = 1 - (blendMapColour.r + blendMapColour.g + blendMapColour.b); //gets black value from the blendmap for each triangle primitive.
	
	vec4 totalColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec4 blackColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec4 redColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec4 greenColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec4 blueColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	vec3 bumpNormal = vec3(0.0f, 0.0f, 0.0f);
	vec3 bumpNormal1 = vec3(0.0f, 0.0f, 0.0f);
	vec3 bumpNormal2 = vec3(0.0f, 0.0f, 0.0f);
	vec3 bumpNormal3 = vec3(0.0f, 0.0f, 0.0f);
	vec3 bumpNormal4 = vec3(0.0f, 0.0f, 0.0f);
	
	
	//trails
	
	if(blendMapColour.r > 0.0f && blendMapColour.g > 0.0f && blendMapColour.b > 0.0f){
		//discard;
	}
	else{
		if(blackColourAmount > 0.0f && blendMapColour.g == 0.0f){
			blackColour = texture(diffuseTex3, scaledTexCoord) * blackColourAmount; //backgroundTexture which goes on black
			
			totalColour = blackColour;
			
			bumpNormal3 = texture(bumpTex3, scaledTexCoord).rgb;
			bumpNormal3 = normalize(TBN * normalize(bumpNormal3 * 2.0 - 1.0));
			
			bumpNormal = bumpNormal3;
			
			//discard;
		}
		else if(blackColourAmount > 0.0f && blendMapColour.g > 0.0f){
			blackColour = texture(diffuseTex3, scaledTexCoord) * blackColourAmount; //backgroundTexture which goes on black
			greenColour = texture(diffuseTex1, scaledTexCoord) * blendMapColour.g; //gTexture - goes on green
			
			totalColour = mix(greenColour, blackColour, 1-blendMapColour.g);
			
			bumpNormal3 = texture(bumpTex3, scaledTexCoord).rgb;
			bumpNormal3 = normalize(TBN * normalize(bumpNormal3 * 2.0 - 1.0));
			
			bumpNormal1 = texture(bumpTex1, scaledTexCoord).rgb;
			bumpNormal1 = normalize(TBN * normalize(bumpNormal1 * 2.0 - 1.0));
			
			bumpNormal = mix(bumpNormal1, bumpNormal3, 1-blendMapColour.g);
		}
		else{
			if(blendMapColour.r > 0.0f && blendMapColour.b == 0.0f){
				redColour = texture(diffuseTex2, scaledTexCoord) * blendMapColour.r; //rTexture - goes on red
				
				totalColour = redColour;
				
				bumpNormal2 = texture(bumpTex2, scaledTexCoord).rgb;
				bumpNormal2 = normalize(TBN * normalize(bumpNormal2 * 2.0 - 1.0));
				
				bumpNormal = bumpNormal2;
			}	
			else if(blendMapColour.r > 0.0f && blendMapColour.b > 0.0f){
				redColour = texture(diffuseTex2, scaledTexCoord) * blendMapColour.r; //rTexture - goes on red
				blueColour = texture(diffuseTex4, scaledTexCoord) * blendMapColour.b; //bTexture - goes on blue
				
				totalColour = mix(redColour, blueColour, 1-blendMapColour.r);
				
				bumpNormal2 = texture(bumpTex2, scaledTexCoord).rgb;
				bumpNormal2 = normalize(TBN * normalize(bumpNormal2 * 2.0 - 1.0));
				
				bumpNormal4 = texture(bumpTex4, scaledTexCoord).rgb;
				bumpNormal4 = normalize(TBN * normalize(bumpNormal4 * 2.0 - 1.0));
				
				bumpNormal = mix(bumpNormal2, bumpNormal4, 1-blendMapColour.r);
			}
			else{
				if(blendMapColour.g > 0.0f && blendMapColour.b == 0.0f){
					greenColour = texture(diffuseTex1, scaledTexCoord) * blendMapColour.g; //gTexture - goes on green
					
					totalColour = greenColour;
					
					bumpNormal1 = texture(bumpTex1, scaledTexCoord).rgb;
					bumpNormal1 = normalize(TBN * normalize(bumpNormal1 * 2.0 - 1.0));
					
					bumpNormal = bumpNormal1;
				}
				else if(blendMapColour.g > 0.0f && blendMapColour.b > 0.0f){
					greenColour = texture(diffuseTex1, scaledTexCoord) * blendMapColour.g; //gTexture - goes on green
					blueColour = texture(diffuseTex4, scaledTexCoord) * blendMapColour.b; //bTexture - goes on blue
					
					totalColour = mix(blueColour, greenColour, 1-blendMapColour.b);
					
					bumpNormal1 = texture(bumpTex1, scaledTexCoord).rgb;
					bumpNormal1 = normalize(TBN * normalize(bumpNormal1 * 2.0 - 1.0));
					
					bumpNormal4 = texture(bumpTex4, scaledTexCoord).rgb;
					bumpNormal4 = normalize(TBN * normalize(bumpNormal4 * 2.0 - 1.0));
					
					bumpNormal = mix(bumpNormal4, bumpNormal1, 1-blendMapColour.b);
				}
				else{
					if(blendMapColour.b > 0.0f){
						blueColour = texture(diffuseTex4, scaledTexCoord) * blendMapColour.b; //bTexture - goes on blue
						
						totalColour = blueColour;
						
						bumpNormal4 = texture(bumpTex4, scaledTexCoord).rgb;
						bumpNormal4 = normalize(TBN * normalize(bumpNormal4 * 2.0 - 1.0));
						
						bumpNormal = bumpNormal4;
					}
				}
			}
		}
	}
	
	vec4 diffuse = vec4(totalColour.rgb, 1.0f);

	float lambert = max(dot(incident, bumpNormal), 0.0f); //brightness
	float distance = length(lightPos - IN.worldPos);
	float attenuation = 1.0f - clamp(distance / lightRadius, 0.0, 1.0);

	float specFactor = clamp(dot(halfDir, bumpNormal), 0.0, 1.0);
	specFactor = pow(specFactor, 128.0); //60.0

	vec3 surface = (diffuse.rgb * lightColour.rgb);
	fragColour.rgb = surface * lambert * attenuation; //diffuse!
	fragColour.rgb += (lightColour.rgb * specFactor)* attenuation; // * 0.70; //specularity! 0.70(now commented) is multiplied to lower the brightness. it's 0.0 to 1.0 where 0.0 is no brightness!
	fragColour.rgb += surface * 0.1f; //ambient!
	fragColour.a = diffuse.a;
}