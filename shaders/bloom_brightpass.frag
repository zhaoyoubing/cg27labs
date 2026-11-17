// bloom_brightpass.frag
#version 420

in vec2 uv;

// scene texture generated in the scene render pass
layout (binding = 8) uniform sampler2D texScene;

uniform float uThreshold = 0.9;

out vec4 colour_out;

void main()
{
    // scene colour
    vec4 colour = texture(texScene, uv);

    // calculate pixel brightness from RGB colour
    float brightness = dot(colour.rgb, vec3(0.2126, 0.7152, 0.0722));

    // keep the colour only if brightness greater than threshold
    if(brightness > uThreshold)
        colour_out = vec4(colour.rgb, 1.0);
    else
        colour_out = vec4(0.0, 0.0, 0.0, 1.0);
}