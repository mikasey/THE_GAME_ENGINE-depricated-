#pragma once
#include "glad/glad.h"

class MeshObject
{
private:
	float* meshVerteces = nullptr;
	size_t meshVertecesSize = 0;
	unsigned int* meshIndeces = nullptr;
	size_t meshIndecesSize = 0;

	unsigned int glVAO = 0;
	unsigned int glVBO = 0;
	unsigned int glEBO = 0;

	// for now all atributes are assumed to be floats, no need for other ones yet
	unsigned int atributesSizeCount[16]{};

public:
	MeshObject(float* verteces, size_t vertSize, unsigned int* indeces, size_t indSize);
	int setAtribute(unsigned char loc, int size); // location range 0-15 size range 1-4
	int instantiate();
	void draw();
};

