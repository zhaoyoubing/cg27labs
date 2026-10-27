#version 410
layout (location = 0) out vec3 gPos;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

in vec2 uv;
in vec3 fragPos;
in vec3 fragNormal;

uniform sampler2D baseColourMap;
//uniform sampler2D texture_specular1;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPos = fragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(fragNormal);
    
    // and the diffuse per-fragment color
    gAlbedo.rgb = texture(baseColourMap, uv).rgb;
    
    // store specular intensity in gAlbedoSpec's alpha component
    //gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
}