#version 330 core

out vec4 fragColor;
in vec3 norm;
in vec3 uv;
in vec3 wordPos;
in vec4 color;

uniform vec3 lightPos;
uniform float shininessStrength;
uniform float shininess;
uniform float opacity;

uniform vec3 diffuseCol;
uniform vec3 specularCol;
uniform vec3 ambientCol;
uniform vec3 emissiveCol;
uniform vec3 camPos;

#define AMBIENT_TEXMASK (1 << 0)
#define AMBIENT_COLMASK (1 << 1)
#define DIFFUSE_TEXMASK (1 << 2)
#define DIFFUSE_COLMASK (1 << 3)
#define SPECULAR_TEXMASK (1 << 4)
#define SPECULAR_COLMASK (1 << 5)

uniform int textureMask;
uniform sampler2D ambientText;
uniform sampler2D diffuseText;
uniform sampler2D specularText;

void main()
{
    vec3 lightDir = normalize(lightPos - wordPos.xyz);

    vec3 ambient, diffuse, specular = vec3(1.0f);

    // Ambient calculation
    if ((textureMask & AMBIENT_COLMASK) != 0)
        ambient = ambientCol;
    if ((textureMask & AMBIENT_TEXMASK) != 0)
        ambient *= texture(ambientText, uv.xy).rgb;

    //Diffuse calculation
    float dif = max(dot(norm, lightDir), 0.0);
    if ((textureMask & DIFFUSE_COLMASK) != 0)
        diffuse = diffuseCol;
    if ((textureMask & DIFFUSE_TEXMASK) != 0)
        diffuse *= texture(diffuseText, uv.xy).rgb;
    diffuse *= dif;

    // Specular calculation
    vec3 viewDir = normalize(camPos - wordPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = max(dot(reflectDir, viewDir), 0.0);
    spec = pow(spec, shininess) * shininessStrength;
    if ((textureMask & SPECULAR_COLMASK) != 0)
        specular = specularCol;
    if ((textureMask & SPECULAR_TEXMASK) != 0)
        specular *= texture(specularText, uv.xy).rgb;
    specular *= spec;

    // Combine diffuse and specular
    vec3 result = ambient + diffuse + specular + emissiveCol;
    fragColor = vec4(result, 1.0f);
}
