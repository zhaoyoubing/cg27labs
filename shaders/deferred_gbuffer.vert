#version 450 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 a_uv;


uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

//out vec3 fragPos;
out vec2 uv;
out vec3 fragNormal;


void main()
{
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);

    //fragPos = vec3(uModel * vec4(aPos, 1.0));
    uv = a_uv;
    
    mat3 matNormal = transpose(inverse(mat3(uModel)));
    fragNormal = matNormal * aNormal;
}