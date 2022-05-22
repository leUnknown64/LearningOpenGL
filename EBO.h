#ifndef EBO_H
#define EBO_H

#include<glad/glad.h>

class EBO {
public:
	// ID reference of EBO
	GLuint ID;
	// Generates EBO and links it to indices
	EBO(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif