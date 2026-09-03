#version 460

// vertex attributes
in layout(location=0) vec3 aPos;
in layout(location=1) vec3 normal;
in layout(location=2) vec2 uv_in;

// uniforms
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

// the colour to the fragment shader
out vec2 uv;

void main()
{
    // convert to homogeneous coordinate
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0); 

    uv = uv_in;
}