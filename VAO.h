#ifndef VAO_H
#define VAO_H

#include<glad/glad.h>
#include "VBO.h"

class VAO {
public:
	// ID reference of VAO
	GLuint ID;
	// Generates a VAO ID
	VAO();

	// Links a VBO to the VAO using a layout
	void linkVBO(VBO& VBO, GLuint layout);
	void Bind();
	void Unbind();
	void Delete();
};

#endif