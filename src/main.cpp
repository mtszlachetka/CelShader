
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "shader_manager.hpp"
#include "mesh_manager.hpp"
#include "camera.hpp"

unsigned step = 0;

void process_input(GLFWwindow* window, camera& cam) {
	float angleSpeed = 0.1f;
	float moveSpeed = 0.1f;
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
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.m_dir = glm::vec3(glm::eulerAngleY(angleSpeed) * glm::vec4(cam.m_dir, 0));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.m_dir = glm::vec3(glm::eulerAngleY(-angleSpeed) * glm::vec4(cam.m_dir, 0));
	
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) step ++;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && step > 0) step--;

	// cam.m_dir = glm::normalize(glm::vec3(0.) - cam.m_pos);

	cam.rebase();
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1000, 1000, "cel shading example", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
        return 1;
    }
    glViewport(0, 0, 1000, 1000);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	render_context ship = s_mesh_manager.load("../models/spaceship.obj");

	GLuint default_vert = s_shader_manager.create_shader(GL_VERTEX_SHADER, "../shaders/default.vert");
	GLuint default_frag = s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/default.frag");
	GLuint contour_vert = s_shader_manager.create_shader(GL_VERTEX_SHADER, "../shaders/contour.vert");
	GLuint contour_frag = s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/contour.frag");

	GLuint default_program = s_shader_manager.create_program( {default_vert, default_frag} );
	GLuint contour_program = s_shader_manager.create_program( {contour_vert, contour_frag} );
	
	camera cam(0.01, 100.f, {0, 0, 1}, {0, 0, -20});
	glm::mat4 model_matrix = glm::eulerAngleXYZ(glm::radians(30.f), glm::radians(45.f), glm::radians(30.f));
	model_matrix = glm::mat4(1);

	while(!glfwWindowShouldClose(window)) {

		process_input(window, cam);

		glClearColor(0.5, 0.5, 0.5, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 camera_matrix = cam.get_camera_matrix();
		glm::mat4 perspective_matrix = cam.get_perspective_matrix();

		glCullFace(GL_BACK);
		glUseProgram(default_program);
		glUniformMatrix4fv(glGetUniformLocation(default_program, "model_matrix"), 1, GL_FALSE, (float*)&model_matrix);
		glUniformMatrix4fv(glGetUniformLocation(default_program, "camera_matrix"), 1, GL_FALSE, (float*)&camera_matrix);
		glUniformMatrix4fv(glGetUniformLocation(default_program, "perspective_matrix"), 1, GL_FALSE, (float*)&perspective_matrix);
		glUniform3fv(glGetUniformLocation(default_program, "camera_pos"), 1, (float*)&cam.m_pos);

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
