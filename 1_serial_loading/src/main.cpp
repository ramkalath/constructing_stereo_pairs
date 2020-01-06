/*****************************************************************************
 * Author : Ram
 * Date : 26/Dec/2019
 * Email : ramkalath@gmail.com
 * Breif Description : stereoscopic view using renderbuffers
 * Detailed Description : simple object loading with assimp abstracted. (TODO: ram) abstract window loading using SDL and not glfw; global game settings uniforms uploads; texture loading
 *****************************************************************************/
// GLEW and GLFW includes
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other popular includes
#include <iostream>
#include <chrono>

// User created headers
#include "../include/shader.h"
#include "../include/modelloader.h"
#include "../include/gamesettings.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	// When the user presses the escape key, we set the window should close property to true, and close the application.
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int width = 1240, height = 800;

int main()
{
	GameSettings globalsettings(width, height);
	// glfw stuff ====================================================================
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(width, height, "Lighting Properties", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if(window == nullptr) {
    	std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
    }
	glewExperimental = GL_TRUE; 
	if(glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, globalsettings.width, globalsettings.height);

	Shader objectshader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");
	Shader fbplate_shader("./shaders/fbplate_vertex_shader.vert", "./shaders/fbplate_fragment_shader.frag");

	modelloader ironman("./resources/IronMan/IronMan.obj");
	ironman.modelmatrix = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	ironman.modelmatrix = glm::translate(ironman.modelmatrix, glm::vec3(0.0f, -1.0f, 0.0f));
	ironman.modelmatrix = glm::scale(ironman.modelmatrix, glm::vec3(0.01f, 0.01f, 0.01f));

	// Create framebuffers for the right and left eye
	float fb_plate[] = {-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 
						 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
						 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
						-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
						 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
						-1.0f,  1.0f, 0.0f, 0.0f, 1.0f};

	unsigned int VBO_plate, VAO_plate;
	glGenVertexArrays(1, &VAO_plate);
	glGenBuffers(1, &VBO_plate);
	glBindVertexArray(VAO_plate);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_plate);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fb_plate), fb_plate, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (GLvoid*)(3*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int rboDepthStencil;
	glGenRenderbuffers(1, &rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "\033[0;91m ERROR::FRAMEBUFFER:: Framebuffer is not complete \033[0m\n" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::mat4 model_plate = glm::mat4(1.0f);
	model_plate = glm::translate(model_plate, glm::vec3(0.0f, 0.0f, 1.0f));
	model_plate = glm::scale(model_plate, glm::vec3(2.0f, 2.0f, 0.0f));

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// #####################################################################################################
		// This block is what is to be rendered inside the frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, fbo); 

		glClearColor(0.15f, 0.23f, 0.26f, 1.0f); // creating the turquoise blue color within the framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// ################################################################################
		// render object ---------------------------------------------------------------
		glUseProgram(objectshader.program);

		// Uniforms
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "view"), 1, GL_FALSE, glm::value_ptr(globalsettings.view));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "projection"), 1, GL_FALSE, glm::value_ptr(globalsettings.projection_perspective));
		glUniform3f(glGetUniformLocation(objectshader.program, "LightDirection"), 0.0f, 0.0f, -10.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightAmbient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightDiffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightSpecular"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "CameraPosition"), 0.0f, 0.0f, 0.0f);
		
		// render ironman
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "model"), 1, GL_FALSE, glm::value_ptr(ironman.modelmatrix));
		ironman.RenderModel();

		glBindFramebuffer(GL_FRAMEBUFFER, 0); 
		// #####################################################################################################

		// framebuffer rendering
		glUseProgram(fbplate_shader.program);
		glBindVertexArray(VAO_plate);
		glUniformMatrix4fv(glGetUniformLocation(fbplate_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_plate));
		glUniformMatrix4fv(glGetUniformLocation(fbplate_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(globalsettings.view));
		glUniformMatrix4fv(glGetUniformLocation(fbplate_shader.program, "projection_perspective"), 1, GL_FALSE, glm::value_ptr(globalsettings.projection_perspective));
		glUniform1i(glGetUniformLocation(fbplate_shader.program, "this_texture"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
	glfwTerminate();
    return 0;
}
