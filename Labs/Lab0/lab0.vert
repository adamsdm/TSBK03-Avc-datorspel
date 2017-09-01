/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

in  vec3  in_Position;
in  vec3  in_Normal;
in  vec2  in_TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 vNormal;
out vec3 vPos;
out vec2 frag_texcoord;

void main(void)
{
	vPos = in_Position;
	// Recalculate vertex normal
	vec4 oNormal = projectionMatrix * viewMatrix * vec4(in_Normal, 1.0);


	frag_texcoord=in_TexCoord;	

	// Convert to vec3 and normalize
	vNormal = normalize(mat3(viewMatrix) * in_Normal);

	gl_Position=projectionMatrix*viewMatrix*vec4(in_Position, 1.0);
}

