#version 410

// texture coordinates
in vec2 uv;

// [TODO] T3.2 define a uniform baseColourMap
// the type is sampler2D


// the output pixel colour
out vec4 out_colour;

void main()
{
    // [TODO] T3.3 retrieve RGB colour from the baseColourMap
    // Hint: use texture() with uv and take the rgb components
    // to generate a vec3 colour variable
    
    
    out_colour = vec4(colour, 1.0);
}