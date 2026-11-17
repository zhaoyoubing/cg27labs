// shaders/gaussian_blur.frag
#version 420
  
in vec2 uv;

layout (binding = 8) uniform sampler2D inTex;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

uniform int uPass;

out vec4 outTex;

void main()
{   
    // current fragment's contribution 
    vec3 result = texture(inTex, uv).rgb * weight[0];  
    // size of single texel 
    vec2 tex_offset = 1.0 / textureSize(inTex, 0);     

    if ( uPass == 0) {
        // horizontal blurring on the x direction
        for(int i = 1; i < 5; i++)  {
            // right half of Gaussian bell
            result += texture(inTex, uv + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            // left half of Gaussian bell
            result += texture(inTex, uv - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    } else {
        // vertical blurring on the y direction  
        for(int i = 1; i < 5; i++) {
            // top half of Gaussian bell
            result += texture(inTex, uv + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            // bottom half of Gaussian bell
            result += texture(inTex, uv - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }

    // for debugging
    //result = texture(inTex, uv).rgb;
    outTex= vec4(result, 1.0);
}