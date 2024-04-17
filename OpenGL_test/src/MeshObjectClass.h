#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ShaderClass.h"

class MeshObject
{
private:
	float* meshVerteces = nullptr;
	unsigned int meshVertecesSize = 0;
	unsigned int* meshIndeces = nullptr;
	unsigned int meshIndecesSize = 0;

	glm::mat4 transform;

	unsigned int glVAO = 0;
	unsigned int glVBO = 0;
	unsigned int glEBO = 0;

	// for now all atributes are assumed to be floats, no need for other ones yet
	unsigned int atributesSizeCount[16]{};

public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	MeshObject(float* verteces, unsigned int vertSize, unsigned int* indeces, unsigned int indSize);
	int setAtribute(unsigned char loc, int size); // location range 0-15 size range 1-4
	int instantiate();
	void draw(Shader shader);
};

