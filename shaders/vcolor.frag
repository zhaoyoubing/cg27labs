#version 410

out vec4 out_colour;
in vec3 frag_colour;

void main()
{
    // RGBA
    out_colour = vec4(frag_colour, 1.0);
}