#version 410

in layout(location=0) vec3 aPos;
in layout(location=1) vec3 vcolour;


// the colour to the fragment shader
out vec3 frag_colour;

void main()
{
    // convert to homogeneous coordinate
    gl_Position = vec4(aPos, 1.0); 

    frag_colour = vcolour;
}