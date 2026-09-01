#version 410

in layout(location=0) vec3 aPos;
in layout(location=1) vec3 in_colour;

// MVP matrices for future labs
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

// the colour to the fragment shader
out vec3 vcolour;

void main()
{
    // convert to homogeneous coordinate
    gl_Position = vec4(aPos, 1.0); 

    vcolour = in_colour;
}