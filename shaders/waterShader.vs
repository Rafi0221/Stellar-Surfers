#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct Wave{
    float amplitude;
    float steepness;
    float frequency;
    float speed;
    vec4 direction;
};

uniform int waveNumber;

layout (std140) uniform WaveBuffer{
    Wave data[100];
} waves;

uniform float time;
uniform float radius;

void main()
{
    float sinPart = 0;
    vec3 cosPart = vec3(0);

    float sinPartNorm = 0;
    vec3 cosPartNorm = vec3(0);

    vec3 tangent = vec3(0);

    vec3 normalizedPos = normalize(aPos);

    for(int i = 0; i < waveNumber; i++){
        vec3 waveDir = waves.data[i].direction.xyz;
        vec3 waveDirNormalized = normalize(waveDir);

        float amplitude = waves.data[i].amplitude;
        float steepness = waves.data[i].steepness;
        float frequency = waves.data[i].frequency;
        float speed = waves.data[i].speed;

        vec3 dir = cross(normalizedPos, cross(normalizedPos - waveDirNormalized, normalizedPos));
        float dist = acos(dot(normalizedPos, waveDirNormalized)) * radius;

        sinPart += amplitude * sin(frequency * dist + speed * time);
        cosPart += steepness * amplitude * cos(frequency * dist + speed * time) * dir;

        sinPartNorm += steepness * amplitude * frequency * sin(frequency * dist + speed * time);
        cosPartNorm += dir * amplitude * frequency * cos(frequency * dist + speed * time);

        vec3 tmp = cross(dir, normalizedPos);
        tangent += normalize(tmp);
    }

    tangent = normalize(tangent);

    vec3 surfaceLevel = normalizedPos * radius;
    vec3 waveMovement = normalizedPos * sinPart + cosPart;
    vec3 wavePosition = surfaceLevel + waveMovement;

    vec3 waveNormal = normalizedPos - normalizedPos * sinPartNorm - cosPartNorm;
    waveNormal = mix(normalizedPos, waveNormal, 0.8);

    Normal = normalize(waveNormal);
    Normal = mat3(transpose(inverse(model))) * Normal;

    FragPos = vec3(model * vec4(wavePosition, 1.0));
    TexCoord = aTexCoord;

    gl_Position = projection * view * model * vec4(wavePosition, 1.0);
}
