// Reconstruct View Space Coordinates from Screen Space uv and depth 
// adapted from genAI
vec3 reconViewPos(vec3 screenPos) {
    vec2 uv = screenPos.xy;
    float depth = screenPos.z;

    // 2. Convert screen UV and depth to Normalized Device Coordinates (NDC) [-1, 1]
    vec4 ndcPos = vec4(uv * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);

    // 3. Transform by the inverse projection matrix to get Clip Space coordinates
    vec4 clipPos = uMatInvProj * ndcPos;

    // 4. Perspective divide (divide by w to complete the projection math)
    vec4 viewPos = clipPos / clipPos.w;

    return viewPos.xyz; // This is the 3D position in View Space
}

// ACES tone mapping
vec3 ACESFilm(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}