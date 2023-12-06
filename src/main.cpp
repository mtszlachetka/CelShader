
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "shader_manager.hpp"
#include "mesh_manager.hpp"
#include "camera.hpp"
#include "texture_manager.hpp"

unsigned step = 0;

void process_input(GLFWwindow* window, camera& cam) {
	float angleSpeed = 0.1f;
	float moveSpeed = 0.5f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.m_pos += cam.m_dir * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.m_pos -= cam.m_dir * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		cam.m_pos += cam.m_side * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		cam.m_pos -= cam.m_side * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		cam.m_pos += cam.m_up * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		cam.m_pos -= cam.m_up * moveSpeed;
	

	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) step = 0;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) step = 1;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) step = 2;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) step = 3;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) step = 4;

	cam.m_dir = glm::normalize(glm::vec3(0.) - cam.m_pos);

	cam.rebase();

}

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1600, 900, "cel shading example", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
        return 1;
    }
    glViewport(0, 0, 1600, 900);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	render_context ship = s_mesh_manager.load("../models/spaceship.obj");

	GLuint default_vert = s_shader_manager.create_shader(GL_VERTEX_SHADER, "../shaders/default.vert");
	GLuint default_frag = s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/default.frag");
	GLuint contour_vert = s_shader_manager.create_shader(GL_VERTEX_SHADER, "../shaders/contour.vert");
	GLuint contour_frag = s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/contour.frag");
	GLuint toon_vert = s_shader_manager.create_shader(GL_VERTEX_SHADER, "../shaders/toon.vert");
	GLuint toon_frag = s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/toon.frag");
	GLuint xtoon_vert = s_shader_manager.create_shader(GL_VERTEX_SHADER, "../shaders/xtoon.vert");
	GLuint xtoon_frag = s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/xtoon.frag");
	GLuint stylized_vert = s_shader_manager.create_shader(GL_VERTEX_SHADER, "../shaders/stylized.vert");
	GLuint stylized_frag = s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/stylized.frag");

	GLuint default_program = s_shader_manager.create_program( {default_vert, default_frag} );
	GLuint contour_program = s_shader_manager.create_program( {contour_vert, contour_frag} );
	GLuint toon_program = s_shader_manager.create_program( {toon_vert, toon_frag} );
	GLuint xtoon_program = s_shader_manager.create_program( {xtoon_vert, xtoon_frag} );
	GLuint stylized_program = s_shader_manager.create_program( {stylized_vert, stylized_frag} );

	texture_info specular_tex = s_texture_manager.load_texture("../textures/specular.png", "specular_tex");
	
	camera cam(0.01, 100.f, {0.165627, -0.356907, -0.91934}, {-1.60338, 3.45511, 8.89985});
	cam.set_aspect_ratio(16.f / 9.f);
	glm::mat4 model_matrix = glm::mat4(1);

	glm::vec3 light_pos = {13, 2, 4};

	while(!glfwWindowShouldClose(window)) {
		process_input(window, cam);

		glClearColor(0.5, 0.5, 0.5, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 camera_matrix = cam.get_camera_matrix();
		glm::mat4 perspective_matrix = cam.get_perspective_matrix();

		GLuint active_program;
		switch (step) {
			case 0:
			case 1:
				active_program = default_program;
				break;
			case 2: 
				active_program = toon_program;
				break;
			case 3: 
				active_program = xtoon_program;
				break;
			case 4:
				active_program = stylized_program;
				break;
			default:
				active_program = default_program;
		}

		glCullFace(GL_BACK);
		glUseProgram(active_program);
		glUniformMatrix4fv(glGetUniformLocation(active_program, "model_matrix"), 1, GL_FALSE, (float*)&model_matrix);
		glUniformMatrix4fv(glGetUniformLocation(active_program, "camera_matrix"), 1, GL_FALSE, (float*)&camera_matrix);
		glUniformMatrix4fv(glGetUniformLocation(active_program, "perspective_matrix"), 1, GL_FALSE, (float*)&perspective_matrix);
		glUniform3fv(glGetUniformLocation(active_program, "light_pos"), 1, (float*)&light_pos);
		glUniform1i(glGetUniformLocation(active_program, specular_tex.uniform_name), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, specular_tex.id);

		glBindVertexArray(ship.vertex_array);
		glDrawElements(GL_TRIANGLES, ship.size, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);


		if (step >= 1) {
			glUseProgram(contour_program);
			glCullFace(GL_FRONT);
			glUniformMatrix4fv(glGetUniformLocation(contour_program, "model_matrix"), 1, GL_FALSE, (float*)&model_matrix);
			glUniformMatrix4fv(glGetUniformLocation(contour_program, "camera_matrix"), 1, GL_FALSE, (float*)&camera_matrix);
			glUniformMatrix4fv(glGetUniformLocation(contour_program, "perspective_matrix"), 1, GL_FALSE, (float*)&perspective_matrix);
			glUniform3fv(glGetUniformLocation(contour_program, "camera_pos"), 1, (float*)&cam.m_pos);

			glBindVertexArray(ship.vertex_array);
			glDrawElements(GL_TRIANGLES, ship.size, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}
		
		glUseProgram(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}
