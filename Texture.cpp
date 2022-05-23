#include "Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
	// Assigns texture type to object
	type = texType;

	// Width, height, and number of color channels of the image
	int widthImg, heightImg, numColCh;
	// Flip image to correct position
	stbi_set_flip_vertically_on_load(true);
	// Read image file and store contents into bytes
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	// Generate OpenGL texture object
	glGenTextures(1, &ID);
	// Assign texture to texture unit
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	// Configures algorithm used to resize the image
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Configures how the texture repeats
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Uncomment if using GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	// Assign image to OpenGL texture object
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);

	// Delete image data and unbind the texture
	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) {
	// Gets location of uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Activate shader before changing uniform's value
	shader.Activate();
	// Set uniform's value
	glUniform1i(texUni, unit);
}

void Texture::Bind() {
	glBindTexture(type, ID);
}

void Texture::Unbind() {
	glBindTexture(type, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}