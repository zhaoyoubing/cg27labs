#version 410

// vertex attributes
in layout(location=0) vec3 aPos;
in layout(location=1) vec3 aNormal;
in layout(location=2) vec2 a_uv;

// uniforms
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 fragPos;
out vec3 fragNormal;  // the normal to the fragment shader
out vec2 uv;    // the colour to the fragment shader


void main()
{
    // convert to homogeneous coordinate
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0); 

    // 3d position after modelling transform
    fragPos = vec3(uModel * vec4(aPos, 1.0));

    // surface normal after modelling transform
    //fragNormal = mat3(transpose(inverse(uModel))) * aNormal;  
    fragNormal = uModel * aNormal;  

    uv = a_uv;
}