#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

uniform mat4 camera_matrix;
uniform mat4 model_matrix;
uniform mat4 perspective_matrix;
uniform vec3 camera_pos;
uniform vec3 light_pos;

out vec2 tex_coord;

void main() {
	vec3 world_pos = (model_matrix * vec4(vertex_position, 1)).xyz;
	vec3 normal = normalize(model_matrix * vec4(vertex_normal, 0)).xyz;


	vec3 light_dir = normalize(light_pos - world_pos);
	vec3 view_dir = normalize(camera_pos - world_pos);

	vec3 reflection = reflect(light_dir, normal);

	float z_min = 0.005;
	float z_max = 50;

	float D = 1 - (log(camera_pos.z/z_min)/log(z_max/z_min));

	tex_coord = vec2(dot(normal, light_dir), D);

	gl_Position = perspective_matrix * camera_matrix * model_matrix * vec4(vertex_position, 1);
}