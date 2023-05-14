#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    FragColor = vec4(texture(texture1, TexCoord).rgb, 1.0);
//    FragColor = vec4(vec3(texture(texture1, TexCoord).a - 0.4), 1.0);
//    FragColor = vec4(vec3(0.5), 1.0);
}
