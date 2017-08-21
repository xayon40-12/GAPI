#version 330 core

in vec3 colour;
in vec2 coord;
out vec4 out_Colour;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform int option;

void main()
{
    if((coord.x > 0.5 && option==2) || option==3){
        float z = texture(texture0, coord).r;      // fetch the z-value from our depth texture
        float n = 0.1;                                // the near plane
        float f = 100.0;                               // the far plane
        float c = (2.0 * n) / (f + n - z * (f - n));
        out_Colour = vec4(c, c, c, 1);
    }else
        out_Colour = texture(texture1, coord);
}
