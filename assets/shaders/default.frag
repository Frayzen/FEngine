#version 330 core
out vec4 fragColor;
in vec4 pos;
in vec3 norm;
in vec3 uv;
in vec4 color;

uniform vec3 lightPos;
uniform float shininessStrength;
uniform float shininess;

uniform vec3 diffuseCol;
uniform vec3 specularCol;
uniform vec3 ambientCol;

uniform sampler2D diffuseText;
uniform sampler2D specularText;

void main()
{
    vec3 lightDir = normalize(lightPos - pos.xyz);
    float dif = max(dot(normalize(norm), lightDir), 0);
    vec3 diftex = texture(diffuseText, uv.xy).rgb * dif;
    vec3 diffuse;
    if (length(diftex) != 0)
        diffuse += diftex;
    if (length(diffuseCol) != 0)
        diffuse += diffuseCol;

    // Specular calculation
    vec3 viewDir = normalize(-pos.xyz);
    vec3 reflectDir = reflect(-lightDir, normalize(norm));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess) * shininessStrength;
    vec3 specular = spec * texture(specularText, uv.xy).rgb;

    // Combine diffuse and specular
    vec3 result = ambientCol + diffuse + specular;
    
    fragColor = vec4(result, 1.0);
}
