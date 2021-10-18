#pragma once_flag
#pragma once
#ifndef SHAPE_H
#define SHAPE_H
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

class Shape
{
private:
	unsigned int indices_size;
	GLuint VBO, VAO, EBO;

public:

	~Shape()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
	Shape(std::vector<float> vertices, std::vector<int> indices)
	{
		indices_size = (unsigned int)indices.size();

		/* GENERATE VAO-EBO */
		//GLuint VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, (unsigned int)vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

		const GLuint floatsPerVertex = 3;
		const GLuint floatsPerNormal = 3;
		const GLuint floatsPerUV = 2;

		// Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
		GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

		// Create Vertex Attribute Pointers
		glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		/* GENERATE VAO-EBO */

	}

	void Draw()
	{
		glBindVertexArray(VAO);

		//if (texture_key != TextureKey::null) {
		//	updateTexture(texture_key, textureScaleX, textureScaleY, textureIsExtra);
		//}

		glDrawElements(GL_TRIANGLES,
			indices_size,
			GL_UNSIGNED_INT,
			(void*)0);
		glBindVertexArray(0);
	}
};


#endif
