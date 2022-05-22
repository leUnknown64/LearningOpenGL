#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "shaderClass.h"
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"

/*
 * Based off of OpenGL Crash Course Tutorial on YouTube
 */

// Vertices for equilateral triangle
GLfloat vertices[] =
{
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // lower left corner
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // lower right corner
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // upper corner
	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // inner left
	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // inner right
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // inner down
};

// Indices for vertices order
GLuint indices[] =
{
	0, 3, 5, // lower left triangle
	3, 2, 4, // lower right triangle
	5, 4, 1  // upper triangle
};

int main(){
	// Inititialize GLFW
	glfwInit(); 

	// Tell GLFW what version of OpenGL we are using (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile which only has modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window (width, height, title, fullscreen, other)
	GLFWwindow* window = glfwCreateWindow(800, 800, "My Triangle please enjoy :)", NULL, NULL);
	if (window == NULL){
		// verify window opens correctly
		std::cout << "failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	// Initialize glad
	gladLoadGL();
	// Set OpenGL's viewport in the window
	glViewport(0, 0, 800, 800);

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
	VAO1.linkVBO(VBO1, 0);
	// Unbind all to prevent modification
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	// Main loop

	while (!glfwWindowShouldClose(window)){
		// Update window's background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean back buffer and assign it the new color
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Activate our shader program for OpenGl
		shaderProgram.Activate();
		// Bind VAO so OpenGl uses it
		VAO1.Bind();
		
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		// Poll all GLFW events
		glfwPollEvents();
	}

	// Delete all objects we have created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate(); // Terminate GLFW
	return 0;
}