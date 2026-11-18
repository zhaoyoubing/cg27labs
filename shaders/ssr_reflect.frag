#version 420

layout (location = 0) out vec4 reflectColour;

layout (binding = 1) uniform sampler2D gAlbedo;
layout (binding = 2) uniform sampler2D gNormal;
layout (binding = 3) uniform sampler2D gMat; // Metallic-Roughness
layout (binding = 4) uniform sampler2D gDepth;

uniform float SCR_WIDTH;
uniform float SCR_HEIGHT;

uniform mat4 matInvProj;
uniform mat4 matProj;

uniform float uMaxRayDistance = 100.0f;
uniform int uMaxSteps = 64;
uniform float uStride = 1.0f;
uniform float uThickness = 0.5f;
uniform float uRoughThres = 0.2f;

bool rayIsOutofScreen(vec2 ray){
	return (ray.x > 1 || ray.y > 1 || ray.x < 0 || ray.y < 0) ? true : false;
}

vec3 rayMarch(vec3 rayPos, vec3 dir, int iterationCount){
	float sampleDepth;
	vec3 hitColor = vec3(0);
	bool hit = false;

	for(int i = 0; i < iterationCount; i++){
		rayPos += dir * uStride;
		if(rayIsOutofScreen(rayPos.xy)){
			break;
		}

		sampleDepth = texture(gDepth, rayPos.xy).r;
		float depthDif = rayPos.z - sampleDepth;
		if(depthDif >= 0 && depthDif < 0.00001){ //we have a hit
			hit = true;
			hitColor = texture(gAlbedo, rayPos.xy).rgb;
			break;
		}
	}
	return hitColor;
}

void main(){
	//float maxRayDistance = 100.0f;

	// View Space ray calculation
	vec3 screenPos;
	screenPos.xy = vec2(gl_FragCoord.x / SCR_WIDTH,  gl_FragCoord.y / SCR_HEIGHT);
	vec3 normalView = texture(gNormal, screenPos.xy).rgb;	
	float depth = texture(gDepth, screenPos.xy).r;	// 0< <1
    if (depth >= 1.0) discard; 

	screenPos.z = depth;		
	vec4 positionView = matInvProj * vec4(screenPos * 2 - vec3(1), 1);
	positionView /= positionView.w;

	vec3 reflectionView = normalize(reflect(positionView.xyz, normalView));
	if(reflectionView.z > 0){
		reflectColour = vec4(0,0,0,1);
		return;
	}

	vec3 rayEndPositionView = positionView.xyz + reflectionView * uMaxRayDistance;

	// Texture Space ray calculation
	vec4 rayEndPositionTexture = matProj * vec4(rayEndPositionView,1);
	rayEndPositionTexture /= rayEndPositionTexture.w;
	rayEndPositionTexture.xyz = (rayEndPositionTexture.xyz + vec3(1)) / 2.0f;
	vec3 rayDirectionTexture = rayEndPositionTexture.xyz - screenPos;

	ivec2 pixelStartPos = ivec2(screenPos.x * SCR_WIDTH, screenPos.y * SCR_HEIGHT); 
	ivec2 pixelEndPos = ivec2(rayEndPositionTexture.x * SCR_WIDTH, rayEndPositionTexture.y * SCR_HEIGHT); 
	ivec2 pixelDistance = pixelEndPos - pixelStartPos;
	int pixelMaxDistance = max(abs(pixelDistance.x), abs(pixelDistance.y)) / 2;
	rayDirectionTexture /= max(pixelMaxDistance, 0.001f);

	// ray marching
	vec3 outColour = rayMarch(screenPos, rayDirectionTexture, pixelMaxDistance);
	reflectColour = vec4(outColour, 1);
}
