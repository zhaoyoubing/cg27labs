#version 420

layout(binding = 5) uniform samplerCube cubemap;

in vec3 sampleDir; // direction vector representing a 3D texture coordinate

out vec4 out_colour;

void main()
{             
    out_colour = texture(cubemap, sampleDir);
    //out_colour = vec4(sampleDir, 1.0);
}  
