#version 430 core

in vec3 light_dir_TS;

out vec4 out_color;

void main() {
	vec3 normal = vec3(0, 0, 1);
	vec3 light_dir = normalize(light_dir_TS);
	float prod = dot(normal, light_dir);
	if (prod > 0.5) {
		out_color = vec4(1);
	} else if (prod > 0) {
		out_color = vec4(0.33, 0.33, 0.33, 1);
	} else {
		out_color = vec4(0);
	}
}