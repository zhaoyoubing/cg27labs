#version 420

layout(binding = 5) uniform samplerCube cubemap;

uniform vec3 uViewPos;

/*
uniform sampler2D baseColourMap;
uniform bool bUseBlinn = true;


uniform PointLight uLight;
*/

in vec3 fragPos;
in vec3 fragNormal; // interpolated surface normal
//in vec2 uv; // texture coordinates


// the output pixel colour
out vec4 out_colour;

void main()
{
    // RGBA from texture
    out_colour = texture(cubemap, fragNormal);

}