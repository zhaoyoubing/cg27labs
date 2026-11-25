// volfog_blend.frag
#version 420 core

in vec2 uv;

layout (binding = 8) uniform sampler2D texScene;
layout (binding = 9) uniform sampler2D mapVolFog;

uniform float uVolFogIntensity = 1.0;

out vec4 colour_out;


// ACES tone mapping
vec3 ACESFilm(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main()
{
    // 1. obtain the scene colour and the reflection colour             
    vec3 sceneColour = texture(texScene, uv).rgb;      
    vec3 fogColour = texture(mapVolFog, uv).rgb;

    // 2. additive blending
    // vec3 blendColour = sceneColour + F * distFade * ssrColour; 
    vec3 blendColour = sceneColour + uVolFogIntensity * fogColour; 
    
    vec3 result = blendColour;
    
    // 3. HDR Tone Mapping
    result = ACESFilm(result);

    // 4. Gamma correction     
    const float gamma = 2.2;  
    result = pow(result, vec3(1.0 / gamma));
    
    colour_out = vec4(result, 1.0);
}  