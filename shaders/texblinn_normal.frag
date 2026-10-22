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

uniform sampler2D baseColourMap;
uniform sampler2D normalMap;

uniform bool bUseBlinn = true;

uniform PointLight uLight;

in vec2 uv; // texture coordinates

in vec3 tanFragPos;
in vec3 tanLightPos;
in vec3 tanViewPos;


// the output pixel colour
out vec4 out_colour;

void main()
{
    // RGBA from texture
    vec3 colour = texture(baseColourMap, uv).rgb;
    
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, uv).rgb;

    // transform normal vector to range [-1,1]
    vec3 norm = normalize(normal * 2.0 - 1.0); 


    // light direction
    vec3 lightDir = normalize(tanLightPos - tanFragPos);

    // 1. ambient
    vec3 ambient = 0.05 * colour;

    // 2. Lambertian diffuse
    float cos_theta = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = cos_theta * colour;
    
    // vectors for specular reflection
    vec3 viewDir = normalize(tanViewPos - tanFragPos);

    vec3 halfvecDir = normalize(lightDir + viewDir);  // used by blinn-phong

    // reflectDir = -lightDir + 2 * dot (lightDir, norm) * norm;
    // reflectDir needs to be normalised if it has not been
    vec3 reflectDir = reflect(-lightDir, norm); // for phong 
    
    // 3. specular
    // the shininess coefficient beta = 32.0 
    float spec = 0.0f; 
    if (bUseBlinn) {
        spec = pow(max(dot(norm, halfvecDir), 0.0), 32.0);
    } else {
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    }

    // assuming a light source with a bright white colour
    vec3 specular = uLight.colour * spec;

    // The final output fragment colour 
    // combination of ambient, diffuse and specular
    out_colour = vec4(ambient + diffuse + specular, 1.0);
    //out_colour = vec4(colour, 1.0);
}