// gaussian_blur.frag
#version 420
  
in vec2 uv;

layout (binding=9) uniform sampler2D texblur;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

uniform int pass;

out vec4 colour_out;

void main()
{   
    // current fragment's contribution 
    vec3 result = texture(texblur, uv).rgb * weight[0];  
    // gets size of single texel 
    vec2 tex_offset = 1.0 / textureSize(texblur, 0);     

    if ( pass % 2 == 0)
    {
        // horizontal on the x direction
        for(int i = 1; i < 5; ++i)
        {
            result += texture(texblur, uv + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(texblur, uv - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        // vertical on the x direction  
        for(int i = 1; i < 5; ++i)
        {
            result += texture(texblur, uv + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(texblur, uv - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }

    // for debugging
    //result = texture(texblur, uv).rgb;
    colour_out = vec4(result, 1.0);
}