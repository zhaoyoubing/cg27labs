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
uniform bool bUseBlinn = true;
uniform vec3 uViewPos;

// hardcoded light
uniform PointLight uLight;


in vec3 fragPos;
in vec3 fragNormal; // interpolated surface normal
in vec2 uv; // texture coordinates


// the output pixel colour
out vec4 out_colour;

void main()
{
    // RGBA from texture
    vec3 colour = texture(baseColourMap, uv).rgb;
    
    // normalised fragNormal
    vec3 norm = normalize(fragNormal);

    // light direction
    vec3 lightDir = normalize(uLight.pos - fragPos);

    // 1. ambient
    vec3 ambient = 0.05 * colour;

    // 2. Lambertian diffuse
    float cos_theta = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = cos_theta * colour;
    
    // vectors for specular reflection
    vec3 viewDir = normalize(uViewPos - fragPos);

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
    vec3 specular = vec3(0.3) * spec;

    // The final output fragment colour 
    // combination of ambient, diffuse and specular
    // warning: colour value can be greater than 1 here
    // needs gamma correction 
    out_colour = vec4(ambient + diffuse + specular, 1.0);
}