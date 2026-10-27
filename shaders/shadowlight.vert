// shadowmap.vert
#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 a_uv;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

uniform mat4 matLightSpace;

out vec3 fragNormal;
out vec3 fragPos;
out vec2 uv;

out vec4 fragPosLightSpace;

void main()
{   
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);

    fragPos = vec3(uModel * vec4(aPos, 1.0));
    
    //mat3 normalMatrix = mat3(model);
    fragNormal = transpose(inverse(mat3(uModel))) * aNormal;

    uv = a_uv;
    
    fragPosLightSpace = matLightSpace * vec4(fragPos, 1.0);
}