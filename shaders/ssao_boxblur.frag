#version 420

out float fragColor;
//out vec4 out_colour;

in vec2 uv;

layout(binding = 3) uniform sampler2D gDepth;

layout (binding = 9) uniform sampler2D mapAO;

uniform int uRadius = 2;

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(mapAO, 0));

    float result = 0.0;
    for (int x = -uRadius; x <= uRadius; ++x)
    {
        for (int y = -uRadius; y <= uRadius; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(mapAO, uv + offset).r;
        }
    }
    
    int kernelSize = 2 * uRadius + 1;
    fragColor = result / (kernelSize * kernelSize);

    //out_colour = vec4(vec3(result / (5.0 * 5.0)), 1.0);
}
