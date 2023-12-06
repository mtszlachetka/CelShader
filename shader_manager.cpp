#include "shader_manager.hpp"
#include <fstream>
#include <iostream>

std::string shader_manager::read_shader(const std::string& path) {
    std::string shader_code;
	std::ifstream file(path, std::ios::in);

	if (!file.good()) {
		throw std::runtime_error("File " + path + " not found.");
	}

	file.seekg(0, std::ios::end);
	shader_code.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shader_code[0], shader_code.size());
	file.close();
	return shader_code;
}

GLuint shader_manager::create_shader(GLenum type, const std::string& path) {
    GLint result = 0;
    GLuint shader = glCreateShader(type);

    std::string source = read_shader(path);

    const GLchar* p_shader_code = source.c_str();
    const GLint code_size = source.size();

    glShaderSource(shader, 1, &p_shader_code, &code_size);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        GLint info_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);
        std::string shader_log;
        shader_log.resize(info_length);
        glGetShaderInfoLog(shader, info_length, nullptr, shader_log.data());
        throw std::runtime_error("Error compiling shader: " + source + shader_log);
    }

    return shader;
}

GLuint shader_manager::create_program(const std::vector<GLuint>& list) {

    GLuint program = glCreateProgram();
    GLint result = 0;

    std::vector<GLuint> shaders;

    for (auto& shader: list) {
        shaders.push_back(shader);
        glAttachShader(program, shader);
    }

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &result);

    for (auto& shader : shaders) {
        glDetachShader(program, shader);
    }

    if (result == GL_FALSE) {
        GLint info_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length);
        std::string program_log;
        program_log.resize(info_length);
        glGetProgramInfoLog(program, info_length, nullptr, program_log.data());
        throw std::runtime_error("Error linking shaders: " + program_log);
    }

    return program;
}

void shader_manager::delete_program(GLuint program) {
    glDeleteProgram(program);
}