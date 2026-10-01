#version 410

in layout(location=0) vec3 aPos;    // Input position from VBO
in layout(location=1) vec3 aColour; // Input colour from VBO

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

// the colour to the fragment shader
out vec3 fragColour;                // Output to the fragment shader

void main()
{
    // convert to homogeneous coordinate
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0); 

    fragColour = aColour;
}