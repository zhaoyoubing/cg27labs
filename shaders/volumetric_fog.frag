// Tutorial example: https://andrewphamdotblog.wordpress.com/2019/10/03/volumetric-lighting/
#version 420

layout (location = 0) out vec4 outColour;

layout (binding = 0) uniform sampler2D gAlbedo;
layout (binding = 1) uniform sampler2D gNormal;
layout (binding = 2) uniform sampler2D gMat; // Metallic-Roughness
layout (binding = 3) uniform sampler2D gDepth;

layout (binding = 5) uniform sampler2D shadowMap;
layout (binding = 6) uniform sampler2D texScene;

// matrices for convertion between View Space and Screen Space
uniform vec3 uEyePos;
uniform mat4 uMatView;
uniform mat4 uMatInvView;
uniform mat4 uMatProj;
uniform mat4 uMatInvProj;
uniform mat4 uMatLightSpace;

// Volumetric Fog parameters
uniform int uNumSteps = 128;

const float sigmaT = 0.1f;
const float G = 0.7f; // Controls how much light will scatter in the forward direction
const float PI = 3.14159265359f;

// traditional point or directional lights
struct Light {
    int type; // 0 : directional, 1 : point
    vec3 pos;
    vec3 dir;
    vec3 colour;
    float intensity;
    
    // Point Light Distance attenuation coefficients
    float a; // constant
    float b; // linear
    float c; // quadratic
};

uniform Light uLight;
vec3 lightDir;

in vec2 uv;



bool rayIsOutofScreen(vec3 ray) {
	return (ray.x > 1 || ray.y > 1 || ray.z > 1 
	       || ray.x < 0 || ray.y < 0 || ray.z < 0) ? true : false;
}

// convert Screen Space position (u, v, depth) to World Space position
vec3 screen2world(vec3 screenPos) {
	vec4 viewPos = uMatInvProj * vec4(screenPos * 2 - vec3(1.0), 1.0);
	viewPos = viewPos / viewPos.w;
    vec4 worldPos = uMatInvView * viewPos;

    return worldPos.xyz; 
}

vec3 world2screen(vec3 worldPos) {
	vec4 clipPos = uMatProj * uMatView * vec4(worldPos, 1.0);
	vec3 ndcPos = clipPos.xyz / clipPos.w;
	vec3 screenPos = (ndcPos + 1.0) * 0.5;
	return screenPos;
}

vec3 world2LightScreen(vec3 worldPos) {

	vec4 posLightClip = uMatLightSpace * vec4(worldPos, 1.0);

    // perspective divide to normalised device coordinates [-1, 1] 
    vec3 posLightNdc = posLightClip.xyz / posLightClip.w;
    
    // NDC to screen space [0,1]
    vec3 posLightScreen = posLightNdc * 0.5 + 0.5;

	return posLightScreen;
}

float shadow(vec4 fragPosLightSpace)
{
    // perform perspective divide 
    // result: normalised device coordinates (NDC)
    // range [-1, 1] for x, y, z
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // get the depth value from the depth map 
    // corresponding to the closest visible part to the light source 
    float closeDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // larger depth means far from the light source 
    // You must use bias with deferred shading to avoid shaodow acne
    float bias = 0.005;
    float shadow = currentDepth - bias > closeDepth  ? 1.0 : 0.0;
    //float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);    
    
    return shadow;
}

// code from the tutorial
// Mie-scattering phase function approximated by the Henyey-Greenstein phase function
float calcScattering(float cosTheta)
{
    return (1.0f - G * G) / (4.0f * PI * pow(1.0f + G * G - 2.0f * G * cosTheta, 1.5f));
}

// IGN, popularized by Jorge Jimenez for Call of Duty
// achieves variation across pixels — and across frames — by evaluating 
// a fast, deterministic dot-product hash on screen coordinates
float InterleavedGradientNoise(vec2 uv) {
    vec3 magic = vec3(0.06711056f, 0.00583715f, 52.9829189f);
    return fract(magic.z * fract(dot(uv, magic.xy)));
}

// It starts from the current fragment position, 
// walk along the view ray direction 
// to check if the sample points are visible by the light source. 
// If visible, accumulate the scattering light, 
// finally return the accumulated volumetric fog colour for blending in the next pass.
vec4 rayMarch(vec3 rayPos, vec3 dir) {
	
	vec4 accColour = vec4(0.0); // black

	float sceneDepth = texture(gDepth, uv).r;

	float transmittance = 1.0;

	float stepSize = length(dir) / uNumSteps;
	dir = normalize(dir);

    // Offset initial step phase by jitter * (step Size) instead of jumping wildly
    float jitter = InterleavedGradientNoise(uv);
    float t = (0.5 + jitter);

    // 1. start from the current fragment position in View Space
	vec3 stepVec = dir * stepSize;
	vec3 raySamplePos = rayPos + stepVec * t;

    // 2. walk along the ray step by step
	for(int i = 0; i < uNumSteps; i++){
		float stepTransmittance = exp(-sigmaT * stepSize);
		float stepIntegral = (1.0 - stepTransmittance) / sigmaT;

        // 2.2 project sample points on the ray to screen space
		vec3 raySampleScreenPos = world2screen(raySamplePos);
		if(rayIsOutofScreen(raySampleScreenPos)) {
			// the starting segments
			raySamplePos += stepVec;
			continue;
		}

		// 2.3 check the current visible scene depth on the sample uv  
        // 2.4 check if the sample still visible by the eye
		if (raySampleScreenPos.z > sceneDepth) {
			break;
		}

		// now visible both by the eye and the light source
		// check if under shadow
		vec4 fragPosLightSpace = uMatLightSpace * vec4(raySamplePos, 1.0);
		float shadowVal = shadow(fragPosLightSpace);

		if (shadowVal < 1.0) {
			// not in shadow
			float cosTheta = dot(dir, -lightDir);
		    accColour.xyz += calcScattering(cosTheta) * uLight.colour * uLight.intensity 
			                * transmittance * stepIntegral;
			//accColour.xyz += uLight.colour; // for debugging
		}

		transmittance *= stepTransmittance;
		if (transmittance < 0.01)
			break;
		
		// 2.1 generate new sample points on the ray
		raySamplePos += stepVec;
	}

	vec3 scene = texture(texScene, uv).rgb;
	return vec4(accColour.xyz + scene, 1.0);
}

void main(){

	float depth = texture(gDepth, uv).r;	// [0, 1]
	
	// 1. recover View Space position from depth
	vec3 fragPosScreen = vec3(uv, depth);
	vec3 fragPos = screen2world(fragPosScreen);

	// fragPosView is the view direction in the view space
	// convert it to worldspace
	vec3 viewDirWorld = fragPos - uEyePos;

	lightDir = normalize(uLight.dir);

	// 3. calculate the accumulated volumetric fog colour by ray marching
	outColour = rayMarch(uEyePos, viewDirWorld);
	
	// for debugging
	// outColour = vec4(texture(gAlbedo, uv).xyz, 1);
	// outColour = vec4(reflectionView, 1);
}
