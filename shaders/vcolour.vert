#version 410

in layout(location=0) vec3 aPos;
in layout(location=2) vec3 normal;
in layout(location=3) vec2 in_uv;

// MVP matrices for future labs
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

// the colour to the fragment shader
// out vec3 vcolour;
out vec2 uv;

void main()
{
    // convert to homogeneous coordinate
    gl_Position = vec4(aPos, 1.0); 

    uv = in_uv;
}