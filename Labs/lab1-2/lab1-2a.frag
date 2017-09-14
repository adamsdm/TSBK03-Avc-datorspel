#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

in vec2 outTexCoord;
in vec3 out_Normal;
in vec3 Ps;
in vec3 Pt;
in vec3 pixPos;  // Needed for specular reflections

uniform sampler2D texUnit;
uniform float factor;

out vec4 out_Color;

void main(void)
{
    vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates
	
	// Calculate gradients here
	float offset = 1.0 / 256.0; // = ds = dt,  texture size, same in both directions

	// from the course book
	// bs = b[s+1, t] - b[s, t]
	// bt = b[s, t+1] - b[s, t]
	vec3 bs = texture(texUnit, outTexCoord + vec2(offset, 0.0)).xyz - texture(texUnit, outTexCoord).xyz;
	vec3 bt = texture(texUnit, outTexCoord + vec2(0.0, offset)).xyz - texture(texUnit, outTexCoord).xyz;
	
	vec3 normal = normalize(out_Normal + factor*(bs*Ps) + factor*(bt*Pt));


	// Simplified lighting calculation.
	// A full solution would include material, ambient, specular, light sources, multiply by texture.
    out_Color = vec4( dot(normal, light));
}