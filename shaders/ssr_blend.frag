// ssr_blend.frag
#version 420 core

layout(binding = 0) uniform sampler2D gAlbedo;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gMaterial;
layout(binding = 3) uniform sampler2D gDepth;

in vec2 uv;

layout (binding = 8) uniform sampler2D texScene;
layout (binding = 9) uniform sampler2D texSSR;

uniform mat4 uMatView;
uniform mat4 uMatInvProj;

//uniform float uMaxRayDistance = 100.0;

out vec4 colour_out;

// convert Screen Space position (u, v, depth) to View Space position
vec3 screen2view(vec3 screenPos) {
	vec4 viewPos = uMatInvProj * vec4(screenPos * 2 - vec3(1.0), 1.0);
	viewPos /= viewPos.w;

    return viewPos.xyz; 
}

float saturate(float x)
{
    return clamp(x, 0.0, 1.0);
}

float fresnelSchlick(float cosTheta, float F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - saturate(cosTheta), 5.0);
}

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
    vec3 ssrColour = texture(texSSR, uv).rgb;
    
    // 2. recover View Space frag position from Screen Space
    float fragScreenDepth = texture(gDepth, uv).r;
    vec3 fragScreenPos = vec3(uv, fragScreenDepth);
    vec3 fragPos = screen2view(fragScreenPos);

    // 3. obtain the normal vector in the View Space
    vec3 normal = mat3(uMatView) * texture(gNormal, uv).rgb;
    normal = normalize(normal);

    // 4. calculate viewing angle
    vec3 viewDir  = normalize(- fragPos);  // eyePos is zero
    float cosTheta = dot(viewDir, normal);

    // 5. calculate fresnel
    float F = fresnelSchlick(cosTheta, 0.05);
    
    // float rayLength = length(fragPos);
    //float distFade = saturate(1.0 - (rayLength / uMaxRayDistance)); // Fades over distance

    // 6. additive blending
    // vec3 blendColour = sceneColour + F * distFade * ssrColour; 
    vec3 blendColour = sceneColour + F * ssrColour; 
    
    vec3 result = blendColour;
    
    // 7. HDR Tone Mapping
    result = ACESFilm(result);

    // 8. Gamma correction     
    const float gamma = 2.2;  
    result = pow(result, vec3(1.0 / gamma));
    
    colour_out = vec4(result, 1.0);
}  