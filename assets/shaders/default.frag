#version 330 core

out vec4 fragColor;
in vec3 norm;
in vec3 uv;
in vec3 wordPos;
in vec4 color;

uniform vec3 lightPos;
uniform float shininessStrength;
uniform float shininess;

uniform vec3 diffuseCol;
uniform vec3 specularCol;
uniform vec3 ambientCol;
uniform vec3 camPos;

#define AMBIENT_TEXMASK (1 << 0)
#define DIFFUSE_TEXMASK (1 << 1)
#define SPECULAR_TEXMASK (1 << 2)

uniform int textureMask;
uniform sampler2D ambientText;
uniform sampler2D diffuseText;
uniform sampler2D specularText;

void main()
{
    vec3 lightDir = normalize(lightPos - wordPos.xyz);


    // Ambient calculation
    vec3 ambient = ambientCol;
    if ((textureMask & AMBIENT_TEXMASK) != 0)
        ambient *= texture(ambientText, uv.xy).rgb;

    //Diffuse calculation
    float dif = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseCol;
    if ((textureMask & DIFFUSE_TEXMASK) != 0)
        diffuse *= texture(diffuseText, uv.xy).rgb;
    diffuse *= dif;

    // Specular calculation
    vec3 viewDir = normalize(camPos - wordPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = max(dot(reflectDir, viewDir), 0.0);
    spec = pow(spec, shininess) * shininessStrength;
    vec3 specular = specularCol;
    if ((textureMask & SPECULAR_TEXMASK) != 0)
        specular *= texture(specularText, uv.xy).rgb;
    specular *= spec;

    // Combine diffuse and specular
    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0f);
}
