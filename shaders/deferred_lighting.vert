#version 450
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 a_uv;

// Hardcoded array of 2D positions for a triangle
const vec2 a_uv[3] = vec2[](
    vec2( 0.0, 0.0), // Top
    vec2( 2.0, 0.0), // Bottom-Left
    vec2( 0.0, 2.0)  // Bottom-Right
);


out vec2 uv;

void main()
{

    uv = a_uv[gl_VertexID];

    uv = vec2(
        (gl_VertexID << 1) & 2,
        gl_VertexID & 2
    );


    gl_Position = vec4(uv * 2.0 - 1.0, 0.0, 1.0);



}