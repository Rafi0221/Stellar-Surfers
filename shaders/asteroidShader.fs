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
in vec2 TexCoord;

uniform float shininess;
uniform vec3 viewPos;
uniform DirLight dirLight;

uniform sampler2D textureDiffuse;
uniform sampler2D textureSpecular;
uniform sampler2D textureNormal;
void main()
{
//    vec3 normal = normalize(Normal);
    vec3 normal = normalize(texture(textureNormal, TexCoord).rgb);

    vec3 lightDir = normalize(-dirLight.direction);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    vec3 ambient = dirLight.ambient * texture(textureDiffuse, TexCoord).rgb;
    vec3 diffuse = dirLight.diffuse * diff * texture(textureDiffuse, TexCoord).rgb;
    vec3 specular = dirLight.specular * spec * texture(textureSpecular, TexCoord).rgb;
    if(diff <= 0.0)
        specular *= diff;

    FragColor = vec4((ambient + diffuse + specular), 1.0f);

}

