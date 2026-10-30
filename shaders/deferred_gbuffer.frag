#version 450

layout (location = 0) out vec4 gAlbedo;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gMat;

in vec2 uv;
in vec3 fragNormal;

uniform sampler2D baseColourMap;
//uniform sampler2D texture_specular1;


void main()
{    

    // per-fragment normals into the gbuffer
    gNormal = normalize(fragNormal);

    // and the diffuse per-fragment color
    gAlbedo = texture(baseColourMap, uv);

    // debug show normal
    //gAlbedo = vec4((gNormal + 1.0) * 0.5, 1.0);
    
    // store specular intensity in gAlbedoSpec's alpha component
    //gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;

}