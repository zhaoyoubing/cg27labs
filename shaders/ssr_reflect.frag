#version 420

layout (location = 0) out vec4 reflectColour;

layout (binding = 0) uniform sampler2D gAlbedo;
layout (binding = 1) uniform sampler2D gNormal;
layout (binding = 2) uniform sampler2D gMat; // Metallic-Roughness
layout (binding = 3) uniform sampler2D gDepth;

uniform mat4 uMatView;
uniform mat4 uMatProj;
uniform mat4 uMatInvProj;

uniform float uMaxRayDistance = 100.0f;
uniform int uMaxSteps = 64;
uniform float uStride = 1.0f;
uniform float uThickness = 0.5f;
uniform float uRoughThres = 0.2f;

uniform bool bDebug = false;

const float screenThres = 0.05;

in vec2 uv;

bool rayIsOutofScreen(vec3 ray){
	return (ray.x > 1 + screenThres || ray.y > 1 + screenThres || ray.z > 1 + screenThres
	       || ray.x < -screenThres || ray.y < -screenThres || ray.z < -screenThres) ? true : false;
}

// convert Screen Space position (u, v, depth) to View Space position
vec3 screen2view(vec3 screenPos) {
	vec4 viewPos = uMatInvProj * vec4(screenPos * 2 - vec3(1.0), 1.0);
	viewPos /= viewPos.w;

    return viewPos.xyz; 
}

vec3 view2screen(vec3 viewPos) {
	vec4 clipPos = uMatProj * vec4(viewPos, 1.0);
	vec3 ndcPos = clipPos.xyz / clipPos.w;
	vec3 screenPos = (ndcPos + 1.0) * 0.5;
	return screenPos;
}

vec4 rayMarch(vec3 rayPos, vec3 dir, float maxDist) {
	
	vec4 hitColor = vec4(0.0); // black for missing
	
	float sceneDepth;
	bool hit = false;
	float dist = 0;

	vec3 stepVec = dir * uStride;
	vec3 raySamplePos = rayPos;

	for(int i = 0; i < uMaxSteps; i++){
		raySamplePos += stepVec;
		dist += uStride;

		if (dist > maxDist) {
			if (bDebug)
				hitColor = vec4(0.5,0.5,0,0.0);
			break;
		}

		vec3 raySampleScreenPos = view2screen(raySamplePos);

		if(rayIsOutofScreen(raySampleScreenPos)) {
			if (bDebug)
				hitColor = vec4(0.5,0,0, 0.0); // for debugging
			break;
		}

		sceneDepth = texture(gDepth, raySampleScreenPos.xy).r;
		vec3 sceneScreenPos = vec3(raySampleScreenPos.xy, sceneDepth);
		vec3 sceneViewPos = screen2view(sceneScreenPos);

		float depthDif = raySamplePos.z - sceneViewPos.z;

		// ray hit
		if(depthDif >= -uThickness && depthDif <= uThickness) { 
			hit = true;
			hitColor = vec4(texture(gAlbedo, sceneScreenPos.xy).rgb, 1.0);
			break;
		}
	}
	return hitColor;
}

void main(){

	
	float depth = texture(gDepth, uv).r;	// [0, 1]
    if (depth >= 1.0) {
		if (bDebug) {
			reflectColour = vec4(0,0,0.5,0.0);
			return;
		}
		discard;
	} 

	// View Space ray calculation
	vec3 screenPos = vec3(uv, depth);
	
	vec4 positionView = uMatInvProj * vec4(screenPos * 2 - vec3(1), 1);
	positionView /= positionView.w;

    vec3 normalView = mat3(uMatView) * texture(gNormal, screenPos.xy).rgb;
	normalView = normalize(normalView);
	vec3 reflectionView = normalize(reflect(positionView.xyz, normalView));

	vec4 outColour = rayMarch(positionView.xyz, reflectionView, uMaxRayDistance);

	reflectColour = outColour;
	
	//reflectColour = vec4(texture(gNormal, uv).xyz, 1);
	//reflectColour = vec4(reflectionView, 1);
}
