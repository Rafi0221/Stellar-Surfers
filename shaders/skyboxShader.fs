// #version 330 core
out vec4 FragColor;

in vec3 cPos;

float star_cutoff(float x) {
	return max((x - 0.84) * 6.25, 0.0);
}
vec3 star_noise(vec3 nPos) {
	return vec3(
		star_cutoff(perlin(50.2 * nPos)),
		star_cutoff(perlin(50.3 * nPos)),
		star_cutoff(perlin(50.4 * nPos)));
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


void main()
{
//    vec3 nPos = normalize(cPos);
//    vec3 teal = vec3(0.015, 0.01, 0.1);
//    // vec3 turb = vec3(turbulence(nPos * 5.0));
//    // inline for better performance?
//    float turbulence_factor =
//            perlin(nPos * 5.0) * 0.5 +
//            perlin(nPos * 10.0) * 0.25 +
//            perlin(nPos * 20.0) * 0.125 +
//            perlin(nPos * 40.0) * 0.0625;
//    vec3 turb = vec3(turbulence_factor);
//    // vec3 stars = star_noise(nPos * 2.0);
//    vec3 stars = vec3(
//            star_cutoff(perlin(100.4 * nPos)),
//            star_cutoff(perlin(100.6 * nPos)),
//            star_cutoff(perlin(100.8 * nPos)));
//    FragColor = vec4((0.87 * teal) + (0.13 * turb) + stars, 1.0);
    FragColor = teal;
}

