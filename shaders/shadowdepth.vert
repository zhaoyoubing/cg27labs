// shadowdepth.vert
#version 410
layout (location = 0) in vec3 aPos;

uniform mat4 uModel;
uniform mat4 uLightView;
uniform mat4 uLightProj;

void main()
{
    gl_Position = uLightProj * uLightView * uModel * vec4(aPos, 1.0);
}  