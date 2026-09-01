#version 410

// the interpolated fragment colour
in vec2 uv;

uniform sampler2D texMap;

// the output pixel colour
out vec4 out_colour;

void main()
{
    // RGBA
    vec3 colour = texture(texMap, uv).rgb;
    out_colour = vec4(colour, 1.0);
}