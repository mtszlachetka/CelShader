#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

uniform mat4 camera_matrix;
uniform mat4 model_matrix;
uniform mat4 perspective_matrix;
uniform vec3 camera_pos;


void main() {
	vec3 world_pos = (model_matrix * vec4(vertex_position, 1)).xyz;
	float distance = length(world_pos - camera_pos);
	mat4 transform = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		vec3(vertex_normal * distance / 250), 1
	);
	gl_Position = perspective_matrix * camera_matrix * transform * model_matrix * vec4(vertex_position, 1.0);
}