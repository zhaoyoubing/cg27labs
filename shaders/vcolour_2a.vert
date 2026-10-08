#version 410

in layout(location=0) vec3 aPos;
in layout(location=1) vec3 aNormal;
in layout(location=2) vec2 aUV;
in layout(location=4) vec3 aColour;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 fragColour;                // Output to the fragment shader

void main()
{
    // convert to homogeneous coordinate
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0); 

    fragColour = aColour;
}