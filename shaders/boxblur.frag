#version 420

out float fragColor;
//out vec4 out_colour;

in vec2 uv;

//layout(binding = 0) uniform sampler2D gAlbedo;
//layout(binding = 1) uniform sampler2D gNormal;
//layout(binding = 2) uniform sampler2D gMaterial;
layout(binding = 3) uniform sampler2D gDepth;

layout (binding = 9) uniform sampler2D texAO;


void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(texAO, 0));
    float result = 0.0;
    for (int x = -2; x <= 2; ++x)
    {
        for (int y = -2; y <= 2; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(texAO, uv + offset).r;
        }
    }
     fragColor = result / (5.0 * 5.0);

    //out_colour = vec4(vec3(result / (5.0 * 5.0)), 1.0);
}
