#version 420

out float fragColor;

in vec2 uv;

layout(binding = 0) uniform sampler2D gAlbedo;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gMaterial;
layout(binding = 3) uniform sampler2D gDepth;

layout(binding = 9) uniform sampler2D texNoise;

//uniform sampler2D uni_texture0;  // gPosition
//uniform sampler2D uni_texture1;  // gNormal
//uniform sampler2D uni_texture2;  // texNoise

uniform float sample_points[192];

uniform int kernelSize;
uniform float radius;
uniform float bias;

const vec2 noiseScale = vec2(1280.0/4.0, 720.0/4.0);

uniform mat4 uMatProj;
uniform mat4 uMatInvProj;

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

void main()
{
    //vec3 fragPos = texture(uni_texture0, uv).xyz;
    vec3 fragPos = reconViewPos(uv);
    vec3 normal = texture(gNormal, uv).rgb;
    vec3 randomVec = texture(texNoise, uv * noiseScale).xyz;

    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;

	for(int i = 0; i < kernelSize * 3; i += 3)
	{
		vec3 samplep = vec3(sample_points[i], sample_points[i + 1], sample_points[i + 2]);
		samplep = TBN * samplep;
		samplep = fragPos + samplep * radius;

		vec4 offset = vec4(samplep, 1.0);
		offset = uMatProj * offset;
		offset.xyz /= offset.w;
		offset.xyz  = offset.xyz * 0.5 + 0.5;

		float sampleDepth = texture(gDepth, offset.xy).z;

		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));

		occlusion += (sampleDepth >= samplep.z + bias ? 1.0 : 0.0) * rangeCheck;
	}
    occlusion = 1.0 - (occlusion / kernelSize);
    
    fragColor = occlusion;
}
