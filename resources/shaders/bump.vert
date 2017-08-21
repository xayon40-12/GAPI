#version 330 core

uniform mat4 MVP;
uniform mat4 model;
uniform mat3 base;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec3 in_BiTangent;
in vec2 in_TexCoord;

out vec3 normal;
out vec3 tangent;
out vec3 biTangent;
out vec3 pos;
out vec2 texCoord;

void main()
{
    gl_Position = MVP * vec4(in_Vertex, 1.0);
    normal = base*in_Normal;
    tangent = base*in_Tangent;
    biTangent = base*in_BiTangent;
    pos = (model*vec4(in_Vertex, 1)).xyz;
    texCoord = in_TexCoord;
}
