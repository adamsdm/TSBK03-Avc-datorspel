/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

in vec3 vNormal;
in vec3 vPos;
in vec2 frag_texcoord;

out vec4 out_Color;

uniform sampler2D exampletexture;
uniform vec3 lightPos;

void main(void)
{
	vec3 cameraPosition = vec3(0.0, 0.0, 1.0);

	vec3 lightColor = vec3(1.0);
	vec3 light = normalize(lightPos);
	vec3 V = normalize(cameraPosition-vPos);
	vec3 L = normalize(lightPos-vPos);
	vec3 R = -reflect(L, vNormal); 
	
	float kd = 1.0;
	float ka = 0.1;
	float ks = 1.0;
	float shinyness = 32.0;

	vec3 ambient  = ka * lightColor;
	vec3 diffuse  = kd * lightColor * max(0.0, dot(vNormal, L));

	// Specular
	float RdotV = clamp(dot(R,V),0.0, 1.0);   //Clamp between 0-1 to prevent reflection on backside
	vec3 specular = ks * pow(RdotV, shinyness) * lightColor;

	vec3 finalColor = ambient + diffuse + specular;

	//out_Color = vec4(finalColor, 1.0);
	out_Color = texture(exampletexture, frag_texcoord) * vec4(finalColor, 1.0);

}

