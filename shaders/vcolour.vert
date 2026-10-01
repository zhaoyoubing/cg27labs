#version 410

in layout(location=0) vec3 aPos;    // Input position from VBO
in layout(location=1) vec3 aColour; // Input colour from VBO


// MVP matrices for future labs
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 fragColour;                // Output to the fragment shader

void main()
{
    // convert position to homogeneous coordinate
    gl_Position = vec4(aPos, 1.0); 

    // [TODO] Tb2.1 pass aColour to output fragColour
}