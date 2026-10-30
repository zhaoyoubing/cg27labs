#version 450

layout(binding = 0) uniform sampler2D gAlbedo;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gMaterial;
layout(binding = 3) uniform sampler2D gDepth;

//out vec4 fragColor;


// for lighting
//in vec3 fragPos;
//in vec3 fragNormal;

in vec2 uv;

//uniform vec3 uLightPos;
//uniform vec3 uViewPos;

//uniform sampler2D baseColourMap;

/*
struct Light
{
    vec4 pos;   // xyz: position, w: light type
    vec4 dir;  // xyz: direction
    vec4 colour;     // rgb: colour, w: intensity
    vec4 params;     // x: range, y: innerCos,
                     // z: outerCos, w: unused
};

// SSBO light
layout(std430, binding = 0) readonly buffer LightBuffer
{
    Light uLights[];
};

*/

struct PointLight {
    vec3 pos;
    vec3 colour;
    float intensity;
    
    // Distance attenuation coefficients
    float a; // constant
    float b; // linear
    float c; // quadratic
};

uniform PointLight uLights[10];

uniform int uLightCount = 1;

out vec4 colour_out;

void main()
{             
    // retrieve data from gbuffer
    // vec3 fragPos = texture(gPos, uv).rgb;
    
    vec3 normal = texture(gNormal, uv).rgb;
    vec3 diffuse = texture(gAlbedo, uv).rgb;

    //float specular = texture(gAlbedo, uv).a;
    
    // then calculate lighting as usual
    /*
    vec3 lighting  = diffuse * 0.1; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);
    for(int i = 0; i < uLightCount; ++i)
    {
        // diffuse
        vec3 lightDir = normalize(uLights[i].pos - fragPos);
        vec3 diffuse = max(dot(normal, lightDir), 0.0) * diffuse * lights[i].colour;
        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
        vec3 specular = uLights[i].colour * spec * specular;
        // attenuation
        float distance = length(uLights[i].pos - fragPos);
        //float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
        //diffuse *= attenuation;
        //specular *= attenuation;
        lighting += diffuse + specular;        
    }
    */

    //colour_out = vec4(lighting, 1.0);
    colour_out = vec4(diffuse, 1.0);
    //colour_out = vec4(normal * 0.5 + 0.5, 1.0);
    //colour_out = vec4(1.0, 0.0, 0.0, 1.0);
    //colour_out = vec4(uv.x, uv.y, 0.0, 1.0);
}