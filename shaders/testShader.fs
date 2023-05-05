//#version 330 core

in vec3 Pos;

out vec4 FragColor;

void main()
{
    FragColor = vec4(vec3(perlin(Pos * 5)), 1.0);
//    FragColor = vec4(vec3(texture(texture1, height).a / 256.0), 1.0);
}
