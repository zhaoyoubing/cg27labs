#version 410

in layout(location=0) vec3 aPos;
in layout(location=1) vec3 in_colour;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProj;

// the colour to the fragment shader
out vec3 vcolour;

void main()
{
    // convert to homogeneous coordinate
    gl_Position = matProj * matView * matModel * vec4(aPos, 1.0); 

    vcolour = in_colour;
}