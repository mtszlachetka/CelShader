#version 430 core

in vec3 light_dir_TS;
in vec3 half_vec_TS;

out vec4 out_color;

void main() {
	vec3 normal = vec3(0, 0, 1);
	vec3 half_vec = normalize(half_vec_TS);
	if (pow(dot(half_vec, normal), 5) > 0.9) {
		out_color = vec4(1);
	} else {
		vec3 light_dir = normalize(light_dir_TS);
		float prod = dot(normal, light_dir);
		if (prod > 0.7) {
			out_color = vec4(0.66, 0.66, 0.66, 1);
		}
		else if (prod > 0.5) {
			out_color = vec4(0.44, 0.44, 0.44, 1);
		} else if (prod > 0) {
			out_color = vec4(0.22, 0.22, 0.22, 1);
		} else {
			out_color = vec4(0);
		}
	}
}