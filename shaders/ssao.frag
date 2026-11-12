// shader based on https://github.com/Baksonator/ssao
#version 420

out float fragColor;
//out vec4 out_color;  // for debugging

in vec2 uv;

layout(binding = 0) uniform sampler2D gAlbedo;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gMaterial;
layout(binding = 3) uniform sampler2D gDepth;

//layout(binding = 9) uniform sampler2D texNoise;


uniform vec3 uSamplePoints[64];

uniform int uKernelSize = 64;
uniform float uRadius = 0.5f;
uniform float uBias = 0.025f;
uniform float uPowerAO = 1.0f;

uniform mat4 uMatProj;
uniform mat4 uMatInvProj;
//uniform vec2 uViewport;

// const vec2 noiseScale = vec2(1280.0/4.0, 720.0/4.0);

// This function is from genAI
vec3 reconViewPos(vec2 uv) {
    // 1. Sample the depth buffer value [0, 1]
    float depth = texture(gDepth, uv).r;
    
    // If depth is 1.0, it's usually the background/skybox (no geometry)
    if (depth >= 1.0) discard; 

    // 2. Convert screen UV and depth to Normalized Device Coordinates (NDC) [-1, 1]
    vec4 ndcPos = vec4(uv * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);

    // 3. Transform by the inverse projection matrix to get Clip Space coordinates
    vec4 clipPos = uMatInvProj * ndcPos;

    // 4. Perspective divide (divide by w to complete the projection math)
    vec4 viewPos = clipPos / clipPos.w;

    return viewPos.xyz; // This is the 3D position in View Space
}

float InterleavedGradientNoise(vec2 uv) {
    vec3 magic = vec3(0.06711056f, 0.00583715f, 52.9829189f);
    return fract(magic.z * fract(dot(uv, magic.xy)));
}

void main()
{
    vec3 fragPos = reconViewPos(uv);
    vec3 normal = texture(gNormal, uv).rgb;

    // 1. Get random rotation angle (0.0 to 1.0) using fragment screen coordinates
    float noiseVal = InterleavedGradientNoise(gl_FragCoord.xy);
    float rotationAngle = noiseVal * 6.2831853f; // Scale to 2 * PI
    vec3 randomVec = vec3(cos(rotationAngle), sin(rotationAngle), 0.0);

    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    float totalSampleFloat = 0.0;

    // comparison must be done in view space
	for(int i = 0; i < uKernelSize; i ++)
	{
		vec3 samplep = uSamplePoints[i];
		samplep = TBN * samplep;
		samplep = fragPos + samplep * uRadius;

		vec4 offset = vec4(samplep, 1.0);
		vec4 offsetProj = uMatProj * offset;
		offsetProj.xyz /= offsetProj.w;
        // from NDC [-1, 1] to depth [0, 1]
		offsetProj.xyz  = offsetProj.xyz * 0.5 + 0.5;

        // Verify sample is inside screen bounds
        if (offsetProj.x >= 0.0 && offsetProj.x <= 1.0 && offsetProj.y >= 0.0 && offsetProj.y <= 1.0)
        {
            // Note: If gDepth is a raw hardware depth buffer, sampleDepth must be 
            // linearized or reconstructed to view-space depth to match samplep.z.

            vec3 fragPosOffset = reconViewPos(offsetProj.xy);
            // visible depth in the view space
            float sampleDepth = fragPosOffset.z;

            float rangeCheck = smoothstep(0.0, 1.0, uRadius / abs(offset.z - sampleDepth));
            // in local shadow
            occlusion += (sampleDepth >= offset.z + uBias ? 1.0 : 0.0) * rangeCheck;

            totalSampleFloat += 1.0;
        }
	}
    // non occlusion
    float non_occlusion = 1.0 - (occlusion / totalSampleFloat);
    non_occlusion = pow(non_occlusion, uPowerAO);
    
    fragColor = non_occlusion;
    //out_color = vec4(vec3(non_occlusion), 1.0);
    //out_color = vec4(texture(gAlbedo, uv).xyz, 1.0);
    //out_color = vec4(vec3(texture(gDepth, uv).z), 1.0);
}
