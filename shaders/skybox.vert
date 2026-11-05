#version 420

layout (location = 0) in vec3 aPos;

out vec3 sampleDir;

uniform mat4 uModel = mat4(1.0);
uniform mat4 uView = mat4(1.0);
uniform mat4 uProj = mat4(1.0);

void main()
{
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);

    sampleDir = aPos;
}  