#include <iostream>
#include "MeshObject.h"

MeshObject::MeshObject(float* verteces, size_t vertSize, unsigned int* indeces, size_t indSize)
{
	meshVerteces = verteces;
	meshVertecesSize = vertSize;
	meshIndeces = indeces;
	meshIndecesSize = indSize;
	
	for (int i = 15; i >= 0; i--) {
		atributesSizeCount[i] = 0;
	}
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
	

	size_t memSizeNow = 0;
	for (int i = 0; i < 16; i++) {
		if (atributesSizeCount[i] != 0) {
			glVertexAttribPointer(i, atributesSizeCount[i], GL_FLOAT, GL_FALSE, 0, (void*)memSizeNow);
			glEnableVertexAttribArray(i);
			memSizeNow += atributesSizeCount[i] * sizeof(float);
		}
	}	

	glBindVertexArray(0);

	return 0;
}

void MeshObject::draw()
{
	glBindVertexArray(glVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int MeshObject::setAtribute(unsigned char loc, int size)
{
	if (loc < 0 || loc > 15 || size < 1 || size > 4) {
		return -1;
	}
	atributesSizeCount[loc] = size;
	return 0;
}