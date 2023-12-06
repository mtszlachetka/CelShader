#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

uniform mat4 camera_matrix;
uniform mat4 model_matrix;
uniform mat4 perspective_matrix;
uniform vec3 camera_pos;

void main() {
	gl_Position = perspective_matrix * camera_matrix * model_matrix * vec4(vertex_position, 1.0);
}