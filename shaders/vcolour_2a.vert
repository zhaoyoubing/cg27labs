#version 410

in layout(location=0) vec3 aPos;
in layout(location=1) vec3 aNormal;
in layout(location=2) vec2 aUV;
in layout(location=3) vec3 in_colour;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

// the colour to the fragment shader
out vec3 vcolour;

void main()
{
    // convert to homogeneous coordinate
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0); 

    vcolour = in_colour;
}