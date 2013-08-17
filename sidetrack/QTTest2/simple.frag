#version 330
 
layout(location = 0, index = 0) out vec4 fragColor;
in vec3 Color;
 
void main( void )
{
    fragColor = vec4(Color, 1.0);
}
