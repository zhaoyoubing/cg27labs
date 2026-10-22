#version 410

// vertex attributes
in layout(location=0) vec3 aPos;
in layout(location=1) vec3 aNormal;
in layout(location=2) vec2 a_uv;
in layout(location=3) vec4 aTangent;

struct DirectionalLight {
    vec3 dir;
    vec3 colour;
    float intensity;
};

// Point Light (e.g., Lightbulb)
struct PointLight {
    vec3 pos;
    vec3 colour;
    float intensity;
    
    // Distance attenuation coefficients
    float a; // constant
    float b; // linear
    float c; // quadratic
};

// uniforms
uniform PointLight uLight;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform vec3 uViewPos;

// out vec3 fragPos;
//out vec3 fragNormal;  // the normal to the fragment shader
out vec2 uv;    // the colour to the fragment shader

out vec3 tanFragPos;
out vec3 tanLightPos;
out vec3 tanViewPos;


void main()
{
    // convert to homogeneous coordinate
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0); 

    // 3d position after modelling transform
    vec3 fragPos = vec3(uModel * vec4(aPos, 1.0));

    // model transform for normal and tangentFragPos
    // you can also directly use mat3(uModel) for rigid body transform
    mat3 normModel = mat3(transpose(inverse(uModel)));

    vec3 T = normalize(normModel * aTangent.xyz);
    vec3 N = normalize(normModel * aNormal);
    vec3 B = aTangent.w * cross(N, T);  // bitangent

    mat3 invTBN = transpose(mat3(T, B, N));
    tanLightPos = invTBN * uLight.pos;
    tanViewPos  = invTBN * uViewPos;
    tanFragPos  = invTBN * fragPos;

    //fragNormal = N;
    uv = a_uv;
}