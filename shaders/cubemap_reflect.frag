#version 420

layout(binding = 5) uniform samplerCube cubemap;

uniform vec3 uViewPos; // eye position

in vec3 fragPos;
in vec3 fragNormal; // interpolated surface normal

out vec4 out_colour; // the output pixel colour

void main()
{
    // the viewing direction pointing to the viewer
    vec3 viewDir = normalize(uViewPos - fragPos);

    // normalised normal
    vec3 norm = normalize(fragNormal);

    //vec3 reflectDir = 2 * dot (norm, viewDir) * norm - viewDir; 

    // reflect() only takes normalised norm
    // reflect() takes an incident vector pointing to the surface
    vec3 reflectDir = reflect(-viewDir, norm);

    // RGBA from cubemap texture
    out_colour = texture(cubemap, reflectDir);

    // initial testing without reflection
    // out_colour = texture(cubemap, fragNormal);
}