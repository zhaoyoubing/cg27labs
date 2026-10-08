#version 410

// vertex attributes
in layout(location=0) vec3 aPos;
in layout(location=1) vec3 aNormal;
in layout(location=2) vec2 a_uv;

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

    // [TODO] 3.1 Passing a_uv to the fragment shader
    
}