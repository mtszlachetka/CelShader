#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_tangent;
layout(location = 4) in vec3 vertex_bitangent;

uniform mat4 camera_matrix;
uniform mat4 model_matrix;
uniform mat4 perspective_matrix;
uniform vec3 camera_pos;
uniform vec3 light_pos;

out vec3 light_dir_TS;
out vec3 half_vec_TS;

void main() {
	vec3 world_pos = (model_matrix * vec4(vertex_position, 1)).xyz;
	vec3 normal = normalize(model_matrix * vec4(vertex_normal, 0)).xyz;
	vec3 tangent = normalize(model_matrix * vec4(vertex_tangent, 0)).xyz;
	vec3 bitangent = normalize(model_matrix * vec4(vertex_bitangent, 0)).xyz;

	mat3 TBN = transpose(mat3(tangent, bitangent, normal));

	vec3 light_dir = normalize(light_pos - world_pos);
	vec3 view_dir = normalize(camera_pos - world_pos);

	vec3 half_vec = normalize(light_dir + view_dir);

	light_dir_TS = TBN * light_dir;
	half_vec_TS = TBN * half_vec;

	gl_Position = perspective_matrix * camera_matrix * model_matrix * vec4(vertex_position, 1);
}