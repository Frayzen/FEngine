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

uniform sampler2D diffuseText;
uniform sampler2D specularText;

void main()
{
    vec3 lightDir = normalize(lightPos);

    float dif = max(dot(norm, lightDir), 0.0);
    vec3 diftex = texture(diffuseText, uv.xy).rgb;
    vec3 diffuse = (diftex + diffuseCol) * dif;

    // Specular calculation
    vec3 viewDir = normalize(camPos - wordPos.xyz);
    vec3 reflectDir = reflect(lightDir, norm);
    float val = max(dot(viewDir, reflectDir), 0.0f);
    float spec;
    if (val == 0 && shininess == 0)
        spec = shininessStrength;
    else
        spec = pow(val, shininess) * shininessStrength;
    vec3 spetex = texture(specularText, uv.xy).rgb;
    vec3 specular = spec * spetex * specularCol;

    // Combine diffuse and specular
    vec3 result = ambientCol * diffuse; //+ specular;
    
    fragColor = vec4(result, 1.0f);
}
