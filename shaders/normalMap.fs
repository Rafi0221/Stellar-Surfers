#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D positionTexture;
uniform float coordDelta;

void main()
{
//    FragColor = texture(positionTexture, TexCoord);
//    FragColor = vec4(vec3(texture(positionTexture, TexCoord).a - 0.4),1.0);
    vec3 val = texture(positionTexture, TexCoord).rgb;
    vec3 posX = texture(positionTexture, TexCoord + vec2(coordDelta, 0.0)).rgb;
    vec3 negX = texture(positionTexture, TexCoord - vec2(coordDelta, 0.0)).rgb;
    vec3 posY = texture(positionTexture, TexCoord + vec2(0.0, coordDelta)).rgb;
    vec3 negY = texture(positionTexture, TexCoord - vec2(0.0, coordDelta)).rgb;

    vec3 normX = cross(negX - val, negY - val);
    vec3 normY = cross(posX - val, posY - val);

    FragColor = vec4(normalize(normX + normY), texture(positionTexture, TexCoord).a);
}
