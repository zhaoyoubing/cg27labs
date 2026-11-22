#version 420

layout (location = 0) out vec4 reflectColour;

layout (binding = 0) uniform sampler2D gAlbedo;
layout (binding = 1) uniform sampler2D gNormal;
layout (binding = 2) uniform sampler2D gMat; // Metallic-Roughness
layout (binding = 3) uniform sampler2D gDepth;

// matrices for convertion between View Space and Screen Space
uniform mat4 uMatView;
uniform mat4 uMatProj;
uniform mat4 uMatInvProj;

// SSR parameters
uniform float uMaxRayDistance = 100.0f;
uniform int uMaxSteps = 64;
uniform float uStride = 1.0f;
uniform float uThickness = 0.5f;
uniform float uRoughThres = 0.2f;

in vec2 uv;

uniform bool bDebug = false;

const float screenThres = 0.05;

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

// It starts from the current fragment position, 
// walk along the reflected ray direction 
// to check if the sample points on the ray intersects with the scene in Screen Space. 
// If there is an intersection (ray hit), 
// then return the colour at the intersection pixel as the reflection colour.
vec4 rayMarch(vec3 rayPos, vec3 dir, float maxDist) {
	
	vec4 hitColor = vec4(0.0); // black for missing
	
	float sceneDepth;
	bool hit = false;
	float dist = 0;

    // 1. start from the current fragment position in View Space
	vec3 stepVec = dir * uStride;
	vec3 raySamplePos = rayPos;

    // 2. walk along the ray step by step
	for(int i = 0; i < uMaxSteps; i++){
		// 2.1 generate new sample points on the ray
		raySamplePos += stepVec;
		dist += uStride;

		if (dist > maxDist) {
			if (bDebug)
				hitColor = vec4(0.5,0.5,0,0.0);
			break;
		}

        // 2.2 project sample points on the ray to screen space
		vec3 raySampleScreenPos = view2screen(raySamplePos);

		if(rayIsOutofScreen(raySampleScreenPos)) {
			if (bDebug)
				hitColor = vec4(0.5,0,0, 0.0); // for debugging
			break;
		}

        // 2.3 check the current visible scene depth on the sample uv  
		// and reconstruct the scene poistion in the View Space
		sceneDepth = texture(gDepth, raySampleScreenPos.xy).r;
		vec3 sceneScreenPos = vec3(raySampleScreenPos.xy, sceneDepth);
		vec3 sceneViewPos = screen2view(sceneScreenPos);

        // 2.4 calcuate the z difference between the sample and the scene
		float depthDif = raySamplePos.z - sceneViewPos.z;

		// 2.5 ray hit if the depth difference is very small
		if(depthDif >= -uThickness && depthDif <= uThickness) { 
			hit = true;
			// returns the screen space scene colour on the sample uv
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
		//discard;
		return;
	} 

	// 1. recover View Space position from depth
	vec3 fragPosScreen = vec3(uv, depth);
	vec4 fragPosNdc = uMatInvProj * vec4(fragPosScreen * 2 - 1.0, 1.0);
	vec3 fragPosView = fragPosNdc.xyz / fragPosNdc.w;

    // 2 obtain surface normal in View Space
    vec3 normalView = mat3(uMatView) * texture(gNormal, fragPosScreen.xy).rgb;
	normalView = normalize(normalView);

	// 3. calculate the reflection Ray in View Space
	vec3 reflectRayView = normalize(reflect(fragPosView, normalView));

    // 4. ray marching to query the reflection colour from screen space
	vec4 outColour = rayMarch(fragPosView, reflectRayView, uMaxRayDistance);

	reflectColour = outColour;
	
	//reflectColour = vec4(texture(gNormal, uv).xyz, 1);
	//reflectColour = vec4(reflectionView, 1);
}
