// shader based on https://github.com/Baksonator/ssao
#version 420

out float fragColor;
//out vec4 out_color;  // for debugging

in vec2 uv;

layout(binding = 0) uniform sampler2D gAlbedo;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gMaterial;
layout(binding = 3) uniform sampler2D gDepth;


uniform vec3 uSamplePoints[64];

uniform int uKernelSize = 64;
uniform float uRadius = 0.5f;
uniform float uBias = 0.025f;
uniform float uPowerAO = 1.0f;

uniform mat4 uMatView;
uniform mat4 uMatProj;
uniform mat4 uMatInvProj;

// convert Screen Space position (u, v, depth) to View Space position
vec3 screen2view(vec3 screenPos) {
	vec4 viewPos = uMatInvProj * vec4(screenPos * 2 - vec3(1.0), 1.0);
	viewPos /= viewPos.w;

    return viewPos.xyz; 
}

// convert View Space position (u, v, depth) to Screen Space position
vec3 view2screen(vec3 viewPos) {
	vec4 clipPos = uMatProj * vec4(viewPos, 1.0);
	vec3 ndcPos = clipPos.xyz / clipPos.w;
	vec3 screenPos = (ndcPos + 1.0) * 0.5;
	return screenPos;
}

bool isOutofScreen(vec3 screenPos){
	return (screenPos.x > 1 || screenPos.y > 1  || screenPos.z > 1
	       || screenPos.x < 0 || screenPos.y < 0 || screenPos.z < 0) ? true : false;
}

float InterleavedGradientNoise(vec2 uv) {
    vec3 magic = vec3(0.06711056f, 0.00583715f, 52.9829189f);
    return fract(magic.z * fract(dot(uv, magic.xy)));
}

void main()
{
    float fragScreenDepth = texture(gDepth, uv).r;
    vec3 fragScreenPos = vec3(uv, fragScreenDepth);
    vec3 fragViewPos = screen2view(fragScreenPos);

    vec3 normal = mat3(uMatView) * texture(gNormal, uv).rgb;
    normal = normalize(normal);

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
		vec3 sampleOffset = uSamplePoints[i];
		sampleOffset = TBN * sampleOffset;
		vec3 sampleViewPos = fragViewPos + sampleOffset * uRadius;

        vec3 sampleScreenPos = view2screen(sampleViewPos);

        // Verify sample is inside screen bounds
        if (isOutofScreen(sampleScreenPos))
            continue;
        
        // reconstruct scene view space position at the sample pixel
        float sceneScreenDepth = texture(gDepth, sampleScreenPos.xy).r;
        vec3 sceneScreenPos = vec3(sampleScreenPos.xy, sceneScreenDepth);
        vec3 sceneViewPos = screen2view(sceneScreenPos);

        // visible depth in the view space
        float sceneViewDepth = sceneViewPos.z;

        float rangeCheck = smoothstep(0.0, 1.0, uRadius / abs(sampleViewPos.z - sceneViewPos.z));
        
        // in local shadow
        occlusion += (sceneViewPos.z >= sampleViewPos.z + uBias ? 1.0 : 0.0) * rangeCheck;

        totalSampleFloat += 1.0; 
	}

    // non occlusion
    float non_occlusion = 1.0 - (occlusion / totalSampleFloat);
    non_occlusion = pow(non_occlusion, uPowerAO);
    
    fragColor = non_occlusion;

    // for debugging
    //out_color = vec4(vec3(non_occlusion), 1.0);
    //out_color = vec4(texture(gAlbedo, uv).xyz, 1.0);
    //out_color = vec4(vec3(texture(gDepth, uv).z), 1.0);
}
