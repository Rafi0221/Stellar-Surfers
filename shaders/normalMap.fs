//#version 330 core
#define octaves (15)
#define baseFrequency (0.0001)
#define persistance (0.5)
#define lacunarity (2.0)

out vec4 FragColor;

in vec3 CubePos;

vec3 cubeToSphere(vec3 position){
    float x2 = position.x * position.x;
    float y2 = position.y * position.y;
    float z2 = position.z * position.z;

    float x3 = position.x * sqrt(1 - y2/2.0 - z2/2.0 + z2 * y2 / 3.0);
    float y3 = position.y * sqrt(1 - z2/2.0 - x2/2.0 + z2*x2/3.0);
    float z3 = position.z * sqrt(1 - x2/2.0 - y2/2.0 + x2*y2/3.0);

    return vec3(x3,y3,z3);
}

uniform float multiplier;

void main()
{
//    FragColor = vec4(seed, 1.0f);
    vec3 SpherePos = cubeToSphere(CubePos);

    float value = 0.0f;
    float amplitude = 1.0f;
    float frequency = baseFrequency;
    for(int i = 0; i < octaves; i++){
        value += perlin(SpherePos * frequency);
        frequency *= lacunarity;
        amplitude *= persistance;
    }
    value *= multiplier;
    FragColor = vec4(SpherePos, 1 + (value * 2 - 1.0) * 0.2);
//    FragColor = vec4(vec3(perlin(normalize(SpherePos) * 5)), 1.0);
}
