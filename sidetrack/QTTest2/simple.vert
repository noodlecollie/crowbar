#version 330

in vec3 vertex;
in vec3 color;
out vec3 Color;

void main( void )
{
    Color = color;
    gl_Position = vec4(vertex, 1.0);
}
