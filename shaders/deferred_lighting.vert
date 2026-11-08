#version 420

// Hardcoded array of 2D positions for a triangle
const vec2 a_uv[3] = vec2[](
    vec2(0.0, 0.0), // v0 bottom
    vec2(2.0, 0.0), // v1 right
    vec2(0.0, 2.0)  // v2 top
);


out vec2 uv;

void main()
{

    uv = a_uv[gl_VertexID];

    // uv = vec2( (gl_VertexID << 1) & 2, gl_VertexID & 2 );

    gl_Position = vec4(uv * 2.0 - 1.0, 0.0, 1.0);

}