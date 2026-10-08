#version 410

// the interpolated fragment colour
in vec3 fragColour;

// the output pixel colour
out vec4 outColour;

void main()
{
    // convert RGB to RGBA
    outColour = vec4(fragColour, 1.0);
}