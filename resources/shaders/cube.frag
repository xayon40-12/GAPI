#version 330 core

in vec3 colour;
in vec2 coord;

out vec4 out_Colour;

void main()
{
    float w = 0.01;
    if(coord.x<w || coord.x>1-w || coord.y<w*3.0f/2 || coord.y>1-w*3.0f/2 || (coord.x>1.0f/3-w && coord.x<1.0f/3+w) || (coord.x>2.0f/3-w && coord.x<2.0f/3+w) || (coord.y>1.0f/2-w*3.0f/2 && coord.y<1.0f/2+w*3.0f/2))
        out_Colour = vec4(0, 0, 0, 1);
    else
        out_Colour = vec4(colour, 1);
}
