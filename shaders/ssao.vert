#version 420

in vec3 a_Pos;
in vec2 a_uv;

out vec2 uv;

void main()
{
    uv = a_uv;
    gl_Position = vec4(a_Pos, 1.0);
}
