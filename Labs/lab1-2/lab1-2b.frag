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
    mat3 mvt = transpose(mat3(Ps, Pt, out_Normal)); // mat3(v1,v2,v3) => columns
    vec3 light = mvt * vec3(0.0, 0.7, 0.7); // Light source in view coordinates

	// Calculate gradients here
	float offset = 1.0 / 256.0; // = ds = dt,  texture size, same in both directions

    vec3 bs = texture(texUnit, outTexCoord + vec2(offset, 0.0)).xyz - texture(texUnit, outTexCoord).xyz;
    vec3 bt = texture(texUnit, outTexCoord + vec2(0.0, offset)).xyz - texture(texUnit, outTexCoord).xyz;

	
    // From coursebook: n = [bs, bt, 1]^-1
    vec3 normal = normalize(vec3(factor * bs.x, factor * bt.x, 1.0));
	
    out_Color = vec4( dot(normal, light));
}
