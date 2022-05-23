#include<filesystem>
namespace fs = std::filesystem;

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include "shaderClass.h"
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "Texture.h"

/*
 * Based off of OpenGL Crash Course Tutorial on YouTube
 */

// Vertices for equilateral triangle
GLfloat vertices[] =
{ //          COORDINATES     /    COLORS         /   TEXCOORD  //
	-0.5f, -0.5f, 0.0f,         1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // lower left corner
	-0.5f,  0.5f, 0.0f,         0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // upper left corner
	 0.5f,  0.5f, 0.0f,         0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // upper right corner
	 0.5f, -0.5f, 0.0f,         1.0f, 1.0f, 1.0f,   1.0f, 0.0f  // lower right corner
};

// Indices for vertices order
GLuint indices[] =
// equilateral triangle
{
	0, 2, 1,
	0, 3, 2
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
	GLFWwindow* window = glfwCreateWindow(800, 800, "Texture Time", NULL, NULL);
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

	// Main loop

	while (!glfwWindowShouldClose(window)){
		// Update window's background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean back buffer and assign it the new color
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Activate our shader program for OpenGl
		shaderProgram.Activate();
		// Set uniID's value (always set values after the shader program is activated!!!)
		glUniform1f(uniID, 0.5f);
		// Bind texture
		monkaS.Bind();
		// Bind VAO so OpenGl uses it
		VAO1.Bind();
		
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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