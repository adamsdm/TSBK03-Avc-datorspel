#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

float gaussKernel[25] = float[](1,  4,  6,  4,  1,
                                4,  16, 24, 16, 4,
                                6,  24, 36, 24, 6,
                                4,  16, 24, 16, 4,
                                1,  4,  6,  4,  1 );

void main(void)
{
    int size = 256;
    vec2 tex_offset = 1.0 / textureSize(texUnit, 0); // gets size of single texel

    vec3 result = vec3(0.0);
    // int middle = 2*5 + 2;
    // vec3 result = texture(texUnit, outTexCoord).rgb * gaussKernel[middle];

    int bounds = 2;

    for(int x = -2; x <= 2; x++){
        for(int y = -2; y <= 2; y++){
            //result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(texUnit, outTexCoord + vec2(tex_offset.x * x, tex_offset.y * y)).rgb * gaussKernel[(x+2) + (y+2)*5];
            
        }
    }


    out_Color = vec4(result/size, 1.0);
}