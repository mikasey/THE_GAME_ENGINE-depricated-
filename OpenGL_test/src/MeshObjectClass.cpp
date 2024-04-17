#include <iostream>
#include "MeshObjectClass.h"

MeshObject::MeshObject(float* verteces, unsigned int vertSize, unsigned int* indeces, unsigned int indSize)
{
	meshVerteces = verteces;
	meshVertecesSize = vertSize;
	meshIndeces = indeces;
	meshIndecesSize = indSize;
	
	for (int i = 15; i >= 0; i--) {
		atributesSizeCount[i] = 0;
	}

	position = glm::vec3(0);
	scale = glm::vec3(1);
	rotation = glm::vec3(0);

	transform = glm::mat4(1.0f);
}

int MeshObject::instantiate()
{
	if (meshVerteces == nullptr || meshIndeces == nullptr) {
		return -1;
	}
	glGenVertexArrays(1, &glVAO);
	glBindVertexArray(glVAO);
	glGenBuffers(1, &glVBO);
	glBindBuffer(GL_ARRAY_BUFFER, glVBO);
	glBufferData(GL_ARRAY_BUFFER, meshVertecesSize, meshVerteces, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &glEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndecesSize, meshIndeces, GL_DYNAMIC_DRAW);

	GLsizei stride = 0;
	for (int i = 0; i < 16; i++)
		stride += atributesSizeCount[i] * sizeof(float);
	size_t pointer = 0;
	for (int i = 0; i < 16; i++) {
		if (atributesSizeCount[i] != 0) {
			glVertexAttribPointer(i, atributesSizeCount[i], GL_FLOAT, GL_FALSE, stride, (void*)(pointer));
			glEnableVertexAttribArray(i);
			pointer += atributesSizeCount[i] * sizeof(float);
		}
	}	

	glBindVertexArray(0);

	return 0;
}

void MeshObject::draw(Shader shader)
{
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, position);
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, scale);
	shader.setUniformMat4("objTrans", glm::value_ptr(transform));

	glBindVertexArray(glVAO);
	glDrawElements(GL_TRIANGLES, meshIndecesSize, GL_UNSIGNED_INT, 0);
}

int MeshObject::setAtribute(unsigned char loc, int size)
{
	if (loc < 0 || loc > 15 || size < 1 || size > 4) {
		return -1;
	}
	atributesSizeCount[loc] = size;
	return 0;
}