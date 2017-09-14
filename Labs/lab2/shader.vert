#version 150

//in vec3 in_Color;
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

uniform mat4 matrix;
uniform mat4 Rb1;
uniform mat4 Rb2;
uniform mat4 Tb1;
uniform mat4 Tb2;

out vec4 g_color;
const vec3 lightDir = normalize(vec3(0.3, 0.5, 1.0));

// Uppgift 3: Soft-skinning p� GPU
//
// Flytta �ver din implementation av soft skinning fr�n CPU-sidan
// till vertexshadern. Mer info finns p� hemsidan.

void main(void)
{
	// transformera resultatet med ModelView- och Projection-matriserna
	mat4 M1 = Tb1 * Rb1;
	mat4 M2 = Tb2 * Rb2;

	vec4 w1M1v = in_TexCoord.x * M1 * inverse(Tb1) * vec4(in_Position, 1.0);
	vec4 w2M2v = in_TexCoord.y * M2 * inverse(Tb2) * vec4(in_Position, 1.0);

	// v’m = ∑ wiMiv = w1M1vm + w2M2vm
	vec4 vPrim = (w1M1v + w2M2v);
	gl_Position = matrix * vPrim; // Task 2

	// s�tt r�d+gr�n f�rgkanal till vertex Weights
	vec4 color = vec4(in_TexCoord.x, in_TexCoord.y, 0.0, 1.0);

	// L�gg p� en enkel ljuss�ttning p� vertexarna 	
	float intensity = dot(in_Normal, lightDir);
	color.xyz *= intensity;

	g_color = color;
}

