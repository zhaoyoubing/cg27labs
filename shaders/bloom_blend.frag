// bloom_blend.frag
#version 420 core

in vec2 uv;

layout (binding=8) uniform sampler2D texScene;
layout (binding=9) uniform sampler2D texblur;

uniform float uExposure = 1.0;
uniform float uBloomIntensity = 1.0;

out vec4 colour_out;

void main()
{             
    vec3 sceneColour = texture(texScene, uv).rgb;      
    vec3 blurColour = texture(texblur, uv).rgb;
    
    // additive blending
    vec3 blendColour = sceneColour + blurColour * uBloomIntensity; 
    
    vec3 result = blendColour;
    
    // HDR Tone Mapping
    result = vec3(1.0) - exp(-result * uExposure);
    //Gamma correction     
    const float gamma = 2.2;  
    result = pow(result, vec3(1.0 / gamma));
    
    colour_out = vec4(result, 1.0);
    
    // blur colour preview for debugging
    //colour_out = vec4(blurColour, 1.0);
}  