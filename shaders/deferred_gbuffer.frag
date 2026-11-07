#version 410

layout (location = 0) out vec4 gAlbedo;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gMat;

in vec2 uv;
in vec3 fragNormal;

uniform sampler2D baseColourMap;

void main()
{    

    // per-fragment normals into the gbuffer
    gNormal = normalize(fragNormal);

    // and the diffuse per-fragment color
    gAlbedo = texture(baseColourMap, uv);

    // debug preview normal
    // gAlbedo = vec4((gNormal + 1.0) * 0.5, 1.0);

    // debug preview depth
    gAlbedo = vec4(vec3(gl_FragCoord.z), 1.0);
}