#version 330 core
out vec4 FragColor;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

uniform float shininess;

uniform vec3 viewPos;
uniform DirLight dirLight;

void main()
{
    vec3 normal = normalize(Normal);

    vec3 lightDir = normalize(-dirLight.direction);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 ambient = dirLight.ambient * Color;
    vec3 diffuse = dirLight.diffuse * diff * Color;
    vec3 specular = dirLight.specular * spec * Color;
    if(diff <= 0.0)
        specular *= diff;

    FragColor = vec4((ambient + diffuse + specular), 1.0f);
//    FragColor = vec4(Color, 1.0f);
}
