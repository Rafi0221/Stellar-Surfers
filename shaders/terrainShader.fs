#version 330 core
out vec4 FragColor;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec2 TexCoord;
//in vec3 Normal;
//in vec3 Color;

uniform float shininess;

uniform vec3 viewPos;
uniform DirLight dirLight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D normalMapTexture;
uniform sampler1D colorMapTexture;
void main()
{
    vec3 normal = texture(normalMapTexture, TexCoord).rgb;
    normal = mat3(transpose(inverse(model))) * normal;
    normal = normalize(normal);

//    vec3 color = vec3(1.0 - (texture(normalMapTexture, TexCoord).a - 0.8) * 2.5);
//    vec3 color = normal;

//in range (0.8, 1.2)
    float height = texture(normalMapTexture, TexCoord).a;

    height = (height - 0.8) * 2.5;
    vec3 color = texture(colorMapTexture, height).rgb;

    vec3 lightDir = normalize(-dirLight.direction);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 ambient = dirLight.ambient * color;
    vec3 diffuse = dirLight.diffuse * diff * color;
    vec3 specular = dirLight.specular * spec * color;
    if(diff <= 0.0)
        specular *= diff;

    FragColor = vec4((ambient + diffuse + specular), 1.0f);
//    FragColor = vec4(color, 1.0f);
}
