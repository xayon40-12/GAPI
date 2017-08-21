#version 330 core

uniform vec3 light;
uniform vec3 lightPos;

uniform sampler2D texture0;
uniform sampler2D normalMap;
uniform vec3 eyePos;

in vec3 normal;
in vec3 tangent;
in vec3 biTangent;
in vec3 pos;
in vec2 texCoord;

out vec4 out_Color;

void main()
{
    mat3 TBN = transpose(mat3(normalize(tangent), normalize(biTangent), normalize(normal)));
    float size = distance(eyePos, pos);
    vec3 E = TBN*normalize(eyePos-pos);
    vec3 textureNormale = normalize(texture(normalMap, texCoord).rgb*2.0-1.0);
    vec3 ambiantColor = vec3(0);
    vec3 specularColor = vec3(0);
    
    vec3 l = TBN*normalize(lightPos-pos);
    float cosTheta = clamp(dot(l, textureNormale), 0, 1);
    vec3 R = reflect(-l, textureNormale);
    float cosAlpha = clamp(dot(E, R), 0, 1);
    float length = distance(lightPos,pos)+size;
    
    ambiantColor += texture(texture0, texCoord).rgb*light*cosTheta/(length*length);
    specularColor += texture(texture0, texCoord).rgb*light*pow(cosAlpha, 10)/(length*length);
    
    out_Color = vec4(ambiantColor + specularColor, texture(texture0, texCoord).a);
}
