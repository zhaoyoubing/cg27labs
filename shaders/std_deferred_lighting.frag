#version 420

layout(binding = 0) uniform sampler2D gAlbedo;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gMaterial;
layout(binding = 3) uniform sampler2D gDepth;

layout(binding = 5) uniform sampler2D shadowMap;

in vec2 uv;

/*
// TODO in the future

uniform bool bUseSSBOLights = false;

// SSBO Packed Light
struct Light
{
    vec4 pos;     // xyz: position, w: light type
    vec4 dir;     // xyz: direction
    vec4 colour;  // rgb: colour, w: intensity
    vec4 params;  // x: range, y: innerCos,
                  // z: outerCos, w: unused
};

layout(std430, binding = 0) readonly buffer LightBuffer
{
    Light uSSBOLights[];
};
*/

// traditional point or directional lights
struct Light {
    int type; // 0 : directional, 1 : point
    vec3 pos;
    vec3 dir;
    vec3 colour;
    float intensity;
    
    // Point Light Distance attenuation coefficients
    float a; // constant
    float b; // linear
    float c; // quadratic
};

// max number of lights : 128
#define MAX_LIGHTS 64
uniform Light uLights[MAX_LIGHTS];

uniform int uLightCount = 1;

// shadow mapping
// = lightProj * lightView
uniform mat4 uMatLightSpace;

uniform vec3 uEyePos;
uniform mat4 uMatInvView;
uniform mat4 uMatInvProj;

out vec4 colour_out;

uniform bool bUseShadow = true;
uniform bool bUseToneGamma = false;


// convert Screen Space position (u, v, depth) to View Space position
vec3 screen2view(vec3 screenPos) {
	vec4 viewPos = uMatInvProj * vec4(screenPos * 2 - vec3(1.0), 1.0);
	viewPos = viewPos / viewPos.w;

    return viewPos.xyz; 
}

// convert Screen Space position (u, v, depth) to World Space position
vec3 screen2world(vec3 screenPos) {
	vec4 viewPos = uMatInvProj * vec4(screenPos * 2 - vec3(1.0), 1.0);
	viewPos = viewPos / viewPos.w;
    vec4 worldPos = uMatInvView * viewPos;

    return worldPos.xyz; 
}
// tone mapping
vec3 ACESFilm(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

// lighting
/*
float blinnSpecular(vec3 lightDir, vec3 viewDir, vec3 diffuse, vec3 normal) 
{

}
*/

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide 
    // result: normalised device coordinates (NDC)
    // range [-1, 1] for x, y, z
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // get the depth value from the depth map 
    // corresponding to the closest visible part to the light source 
    float closeDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // larger depth means far from the light source
    //float shadow = currentDepth > closeDepth  ? 1.0 : 0.0;
    
    // You must use bias with deferred shading to avoid shaodow acne
    float bias = 0.005;
    float shadow = currentDepth - bias > closeDepth  ? 1.0 : 0.0;
    //float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);    
    
    return shadow;
}


// code based on from LearnOpenGL.com
// needs to adapt to PBR
void main()
{             
    // screen space frag postion
    float fragScreenDepth = texture(gDepth, uv).r;
    vec3 fragScreenPos = vec3(uv, fragScreenDepth);
    // recover world space frag postion
    vec3 fragPos = screen2world(fragScreenPos);

    // lightProj * lightView
    vec4 fragPosLightSpace = uMatLightSpace * vec4(fragPos, 1.0);
    
    vec3 normal = texture(gNormal, uv).rgb;
    vec3 albedo = texture(gAlbedo, uv).rgb;

    //float specular = texture(gAlbedo, uv).a;
    // hard-coded k_s for Phong/Blinn-Phong
    vec3 k_s = vec3(0.9, 0.7, 0.5);
    
    // then calculate lighting as usual
    vec3 totalLight  = albedo * 0.15; // hard-coded ambient component
    vec3 viewDir  = normalize(uEyePos - fragPos);
    for(int i = 0; i < uLightCount; ++i)
    {
        // diffuse
        vec3 lightDir = normalize(uLights[i].pos - fragPos);
        vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo * uLights[i].colour * uLights[i].intensity;
        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
        
        // specular colour 
        vec3 specular = uLights[i].colour * spec * k_s * uLights[i].intensity;

        // attenuation
        float distance = length(uLights[i].pos - fragPos);
        float attenuation = 1.0 / (uLights[i].a + uLights[i].b * distance + uLights[i].c * distance * distance);
        //diffuse *= attenuation;
        //specular *= attenuation;

        if (bUseShadow) {
            float shadow = ShadowCalculation(fragPosLightSpace);
            totalLight += (1.0 - shadow) * (diffuse + specular);    
        } else {
            totalLight += diffuse + specular;
        }    
    }
    
    vec3 result = totalLight;
    if (bUseToneGamma) {
        result = ACESFilm(result);
        result = pow(result, vec3(1.0 / 2.2));  // Gamma = 2.2
    }

    colour_out = vec4(result, 1.0);



    // debugging code
    //colour_out = vec4(albedo, 1.0);
    //colour_out = vec4(normal * 0.5 + 0.5, 1.0);
    //colour_out = vec4(uv.x, uv.y, 0.0, 1.0);
    //colour_out = vec4(texture(shadowMap, uv).rgb, 1.0);
}