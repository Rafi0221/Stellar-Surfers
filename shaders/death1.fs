// #version 330 core
out vec4 FragColor;

uniform vec3 seed;

in vec3 cPos;

vec3 mult(vec3 a, vec3 b) {
	return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

float star_cutoff(float x) {
	return max((x - 0.87) * 7.69, 0.0);
}
vec4 star_noise(vec3 nPos) {
	return vec4(
		star_cutoff(perlin(50.2 * nPos)),
		star_cutoff(perlin(50.3 * nPos)),
		star_cutoff(perlin(50.4 * nPos)),
		1.0);
}

float turbulence(vec3 pos) {
	float ans = 0.0;
	float size = 1.0;
	float factor = 1.0;
	for (int i = 0; i < 4; i++) {
		factor *= 0.5;
		ans += perlin(pos * size) * factor;
		size *= 2.0;
	}
	return ans;
}

/* float turbulence(vec3 pos, float size) {
	float ans = 0.0;
	float initial_size = size;
	while (size >= 1.0) {
		ans += perlin(pos / size) * size;
		size /= 2.0;
	}
	return (0.5 * ans / initial_size);
} */

void main()
{
    // FragColor = vec4(seed, 1.0f);
	vec3 nPos = normalize(cPos);
	/* vec4 noise1 = vec4(
			perlin(5.0 * nPos), 
			perlin(5.0 * nPos), 
			perlin(5.0 * nPos),
			1.0f);
	vec4 noise2 = vec4(
			perlin(10.0 * nPos), 
			perlin(10.0 * nPos), 
			perlin(10.0 * nPos),
			1.0f);
	vec4 noise3 = vec4(
			perlin(20.0 * nPos), 
			perlin(20.0 * nPos), 
			perlin(20.0 * nPos),
			1.0f);
	vec4 noise4 = vec4(
			perlin(40.0 * nPos), 
			perlin(40.0 * nPos), 
			perlin(40.0 * nPos),
			1.0f);
	vec4 noise5 = vec4(
			perlin(60.0 * nPos), 
			perlin(60.0 * nPos), 
			perlin(60.0 * nPos),
			1.0f); */
	vec4 marble_noise = vec4(
		vec3(abs(sin(/* (nPos.x + nPos.y + nPos.z) * 50.0 + */ perlin(nPos) * 20.0))),
		1.0
	);
	// vec4 noise5 = vec4(vec3(perlin(nPos * 3.0) * perlin(nPos * 10.0) * perlin(nPos * 30.0)), 1.0);
    // FragColor = noise1 * 0.5 + noise2 * 0.5;
	// FragColor = star_noise(nPos);
	// vec4 cloud_noise = (0.35 * noise1) + (0.3 * noise2) + (0.2 * noise3) + (0.1 * noise4) + (0.05 * noise5);
	vec4 teal = vec4(0.015, 0.01, 0.1, 1.0);
	FragColor = vec4(abs(
			sin(vec3(turbulence(nPos)) * 40.0 + 
				nPos * 20.0)), 
	1.0);
}
