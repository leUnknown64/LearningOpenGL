#ifndef VBO_H
#define VBO_H

#include<glad/glad.h>

class VBO {
public:
	// ID reference of VBO
	GLuint ID;
	// Generates VBO and links it to vertices
	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif