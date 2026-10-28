// texblinn.frag
#version 410

// for lighting
in vec3 fragPos;
in vec3 fragNormal;
in vec2 uv;

uniform vec3 uLightPos;
uniform vec3 uViewPos;

uniform sampler2D baseColourMap;

struct Light
{
    vec4 position;   // xyz: position, w: light type
    vec4 direction;  // xyz: direction
    vec4 colour;     // rgb: colour, w: intensity
    vec4 params;     // x: range, y: innerCos,
                     // z: outerCos, w: unused
};

// SSBO light
layout(std430, binding = 0) readonly buffer LightBuffer
{
    Light uLights[];
};

uniform int uLightCount;


out vec4 colour_out;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.14159265 * denom * denom;
    return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;  // direct lighting

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return num / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = geometrySchlickGGX(NdotV, roughness);
    float ggx2 = geometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 BRDF_GGX(vec3 N, vec3 V, vec3 L, vec3 F0, float roughness)
{
    vec3 H = normalize(V + L);

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);

    float D = distributionGGX(N, H, roughness);
    float G = geometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = D * G * F;
    float denominator = 4.0 * NdotV * NdotL + 0.001;
    return numerator / denominator;
}

void main()
{
    vec3 colour = texture(baseColourMap, uv).rgb;

    // 1. diffuse
    vec3 lightDir = normalize(uLightPos - fragPos);
    vec3 norm = normalize(fragNormal);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * colour;
    
    // 2. specular
    vec3 viewDir = normalize(uViewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    // the shininess coefficient beta = 32.0 
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);

    // assuming a light source with a bright white colour
    // specular colour should be decided by metallic and light colour
    vec3 specular = vec3(0.3) * spec;


    vec3 F0 = vec3(0.04); // non-metal base reflectivity
    // roughness should come from map or uniform
    float roughness = 0.1; 
    specular = BRDF_GGX(norm, viewDir, lightDir, F0, roughness);

    // The final output fragment colour 
    // combination of ambient, diffuse and specular
    colour_out = vec4(diffuse + specular, 1.0);
}