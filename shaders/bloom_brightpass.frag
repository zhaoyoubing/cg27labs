// bloom_brightpass.frag
#version 420

in vec2 uv;

// render-to-texture of the scene generated in the 1st pass
layout (binding=8) uniform sampler2D texScene;


uniform float uThreshold = 0.9;

out vec4 colour_out;

void main()
{
    vec4 colour = texture(texScene, uv);

    // check whether fragment output is higher than threshold, if so output as brightness color
    float brightness = dot(colour.rgb, vec3(0.2126, 0.7152, 0.0722));

    // filter the bright part based on a threshold
    if(brightness > uThreshold)
        colour_out = vec4(colour.rgb, 1.0);
    else
        colour_out = vec4(0.0, 0.0, 0.0, 1.0);
}