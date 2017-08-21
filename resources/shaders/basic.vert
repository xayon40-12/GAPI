#version 330 core

in vec3 in_Vertex;
in vec3 in_Colour;

out vec3 colour;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP*vec4(in_Vertex, 1.0);
    colour = in_Colour;
}
