#version 420

layout(binding = 0) uniform sampler2D gAlbedo;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gMaterial;
layout(binding = 3) uniform sampler2D gDepth;

in vec2 uv;

/*
// TODO in the future

uniform bool bUseSSBOLights = false;

// SSBO Packed Light
struct Light
{
    vec4 pos;   // xyz: position, w: light type
    vec4 dir;  // xyz: direction
    vec4 colour;     // rgb: colour, w: intensity
    vec4 params;     // x: range, y: innerCos,
                     // z: outerCos, w: unused
};

layout(std430, binding = 0) readonly buffer LightBuffer
{
    Light uSSBOLights[];
};
*/

// traditional point lights
struct PointLight {
    vec3 pos;
    vec3 colour;
    float intensity;
    
    // Distance attenuation coefficients
    float a; // constant
    float b; // linear
    float c; // quadratic
};

// max number of lights : 128
#define MAX_LIGHTS 128
uniform PointLight uLights[MAX_LIGHTS];

uniform int uLightCount = 1;

uniform vec3 uEyePos;
uniform mat4 uMatInvView;
uniform mat4 uMatInvProj;

out vec4 colour_out;

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

vec3 ACESFilm(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

/*
float blinnSpecular(vec3 lightDir, vec3 viewDir, vec3 diffuse, vec3 normal) 
{

}
*/

// code based on from LearnOpenGL.com
// needs to adapt to PBR
void main()
{             
    // recover screen space frag postion
    float fragScreenDepth = texture(gDepth, uv).r;
    vec3 fragScreenPos = vec3(uv, fragScreenDepth);
    vec3 fragPos = screen2world(fragScreenPos);
    
    vec3 normal = texture(gNormal, uv).rgb;
    vec3 albedo = texture(gAlbedo, uv).rgb;

    //float specular = texture(gAlbedo, uv).a;
    // hard-coded k_s for Phong/Blinn-Phong
    vec3 k_s = vec3(0.9, 0.7, 0.5);
    
    // then calculate lighting as usual
    
    vec3 totalLight  = albedo * 0.05; // hard-coded ambient component
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
        diffuse *= attenuation;
        specular *= attenuation;
        totalLight += diffuse + specular;    
    }
    
    // HDR conversion
    float exposure = 1.0f;
    // exposure tone mapping
    // vec3 mapped = vec3(1.0) - exp(-totalLight * exposure);
    vec3 mapped = ACESFilm(totalLight);
    // Gamma correction
    const float gamma = 2.2;
    mapped = pow(mapped, vec3(1.0 / gamma));

    colour_out = vec4(mapped, 1.0);

    //colour_out = vec4(totalLight, 1.0);
    //colour_out = vec4(albedo, 1.0);
    //colour_out = vec4(normal * 0.5 + 0.5, 1.0);
    //colour_out = vec4(1.0, 0.0, 0.0, 1.0);
    //colour_out = vec4(uv.x, uv.y, 0.0, 1.0);
}