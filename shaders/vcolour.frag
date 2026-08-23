#version 410

// the interpolated fragment colour
in vec3 vcolour;

// the output pixel colour
out vec4 out_colour;

void main()
{
    // RGBA
    out_colour = vec4(vcolour, 1.0);
}