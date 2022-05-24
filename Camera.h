#ifndef CAMERA_H
#define CAMERA_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "shaderClass.h"

class Camera {
public:
	// Main camera vectors
	glm::vec3 Position,
		      Orientation = glm::vec3(0.0f, 0.0f, -1.0f),
		      Up = glm::vec3(0.0f, 1.0f, 0.0f);
	// Window width and height
	int width, height;
	// Camera speed and sensitivity
	float speed = 0.1f, sensitivity = 100.0f;
	// Prevents camera from jumping around on the first left click
	bool firstClick = true;


	Camera(int width, int height, glm::vec3 position);
	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
	void Inputs(GLFWwindow* window);
};

#endif
