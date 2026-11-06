#version 420

layout(binding = 5) uniform samplerCube cubemap;

uniform vec3 uViewPos;

in vec3 fragPos;
in vec3 fragNormal; // interpolated surface normal

// the output pixel colour
out vec4 out_colour;

void main()
{
    vec3 viewDir = normalize(uViewPos - fragPos);
    vec3 norm = normalize(fragNormal);

    //vec3 reflectDir = 2 * dot (norm, viewDir) * norm - viewDir; 
    vec3 reflectDir = reflect(-viewDir, norm);  // reflect() only takes normalised norm
    // RGBA from texture
    out_colour = texture(cubemap, reflectDir);
}