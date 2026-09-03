#version 460

// texture coordinates
in vec2 uv;

uniform sampler2D baseColourMap;

// the output pixel colour
out vec4 out_colour;

void main()
{
    // RGBA
    vec3 colour = texture(baseColourMap, uv).rgb;
    out_colour = vec4(colour, 1.0);
    //out_colour = vec4(1.0, 0.0, 1.0 , 1.0);
}