#include<filesystem>
namespace fs = std::filesystem;

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "shaderClass.h"
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "Texture.h"

/*
 * Based off of OpenGL Crash Course Tutorial on YouTube
 */

// Pyramid
GLfloat vertices[] =
{ //          COORDINATES  /    COLORS            /   TEXCOORD   //
	-0.5f,  0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	  0.0f, 0.0f,
	-0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	  5.0f, 0.0f,
	 0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	  0.0f, 0.0f,
	 0.5f,  0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	  5.0f, 0.0f,
	 0.0f,  0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	  2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

// Globals for window width and height
const int winWidth = 800, winHeight = 800;

int main() {
	// Inititialize GLFW
	glfwInit(); 

	// Tell GLFW what version of OpenGL we are using (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile which only has modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window (width, height, title, fullscreen, other)
	GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Texture Time", NULL, NULL);
	if (window == NULL) {
		// verify window opens correctly
		std::cout << "failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	// Initialize glad
	gladLoadGL();
	// Set OpenGL's viewport in the window
	glViewport(0, 0, winWidth, winHeight);

	// Create our shader using default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Generate and bind Vertex Array Object
	VAO VAO1;
	VAO1.Bind();

	// Generate Vertex Buffer Object and link to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generate Element Buffer Object and link to indices
	EBO EBO1(indices, sizeof(indices));

	// Link VBO to VAO
	VAO1.linkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.linkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.linkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent modification
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Gets ID of uniform called scale in default.vert
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Path for parent directory
	std::string parentDir = fs::current_path().string();
	// Path for texture files
	std::string texPath = "/Resources/Textures/";

	// Load in an image as a texture
	Texture monkaS((parentDir + texPath + "monkaS.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	monkaS.texUnit(shaderProgram, "tex0", 0);

	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	// Main loop

	while (!glfwWindowShouldClose(window)) {
		// Update window's background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Activate our shader program for OpenGl
		shaderProgram.Activate();

		// simple timer to rotate our model on every frame
		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60) {
			rotation += 1.5f;
			prevTime = crntTime;
		}

		// Initialize matrices to the 4x4 indentity matrix
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		// Assign transformations to each matrix
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)(winWidth / winHeight), 0.1f, 100.0f);

		// Outputs our matrices into the Vertex Shader
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		// Set uniID's value (always set values after the shader program is activated!!!)
		glUniform1f(uniID, 0.5f);
		// Bind texture
		monkaS.Bind();
		// Bind VAO so OpenGl uses it
		VAO1.Bind();
		
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		// Poll all GLFW events
		glfwPollEvents();
	}

	// Delete all objects we have created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	monkaS.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate(); // Terminate GLFW
	return 0;
}