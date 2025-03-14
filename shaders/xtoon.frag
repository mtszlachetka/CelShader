#version 430 core

uniform sampler2D specular_tex;

in vec2 tex_coord;

out vec4 out_color;

void main() {

	vec4 tex_color = texture(specular_tex, tex_coord);

	out_color = tex_color;
}