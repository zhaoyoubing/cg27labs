// shadowlight.frag
#version 410

struct DirectionalLight {
    vec3 dir;
    vec3 colour;
    float intensity;
};

// Point Light (e.g., Lightbulb)
struct PointLight {
    vec3 pos;
    vec3 colour;
    float intensity;
    
    // Distance attenuation coefficients
    float a; // constant
    float b; // linear
    float c; // quadratic
};

uniform bool bHasTexture;
uniform sampler2D baseColourMap;
uniform bool bUseBlinn = true;
uniform vec3 uViewPos;
uniform bool bHasShadow;
uniform sampler2D shadowMap;

uniform PointLight uLight;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 uv;
in vec4 fragPosLightSpace;

out vec4 colour_out;

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
    float shadow = currentDepth > closeDepth  ? 1.0 : 0.0;
    
    // You can also use bias
    // float bias = 0.005;
    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);    
    
    return shadow;
}

void main()
{   
    vec3 colour = vec3(1.0, 0.0, 0.0);
    if (bHasTexture)
        colour = texture(baseColourMap, uv).rgb;
    
    // 1. ambient
    vec3 ambient = 0.05 * colour;

    // 2. diffuse
    vec3 lightDir = normalize(uLight.pos - fragPos);
    vec3 norm = normalize(fragNormal);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * colour;
    
    // 3. specular
    vec3 viewDir = normalize(uViewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    // the shininess coefficient beta = 32.0 
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);

    // assuming a light source with a bright white colour
    vec3 specular = uLight.colour * spec;   
    
    // calculate shadow using the light space position
    vec3 lighting = ambient;    
    if (bHasShadow) {
        float shadow = ShadowCalculation(fragPosLightSpace);
        lighting = ambient + (1.0 - shadow) * (diffuse + specular);    
    } else {
        lighting = ambient + diffuse + specular;    
    }
    
    colour_out = vec4(lighting, 1.0);
    //colour_out = vec4(colour, 1.0);
}