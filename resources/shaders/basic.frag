#version 330 core

in vec3 colour;

out vec4 out_Colour;

void main()
{
    out_Colour = vec4(colour, 1);
}
