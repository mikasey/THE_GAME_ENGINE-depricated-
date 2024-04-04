#include <iostream>
#include <cmath>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "ShaderClass.h"
#include "MeshObject.h"

void processInput(GLFWwindow*);

int main() {
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(240*4, 144*4, "TEST!!!", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Window not go brrrrr\n" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Glad has failed you...\n" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 240*4, 144*4);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });

	Shader shader("assets//test.gls");
	shader.use();
	float alphaValue = 1.0f;
	shader.setUniformFloat("Color", 0.7f, 0.3f, 0.0f, alphaValue);

	/* OLD STUFF FOR CREATING MESHES
	float verticesTRIG[] = {
		-0.5f, -0.0f, 0.0f,		// bottom left
		 0.5f, -0.0f, 0.0f,		// bottom right
		 0.0f,  0.8f, 0.0f		// top
	};
	unsigned int VAOtrig;
	glGenVertexArrays(1, &VAOtrig);
	glBindVertexArray(VAOtrig);
	unsigned int VBOtrig;
	glGenBuffers(1, &VBOtrig);
	glBindBuffer(GL_ARRAY_BUFFER, VBOtrig);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTRIG), verticesTRIG, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(NULL);

	float verticesSQARE[] = {
		 0.5f, 0.0f, 0.0f,		// top right
		 0.5f, -0.8f, 0.0f,		// bottom right
		-0.5f, -0.8f, 0.0f,		// bottom left
		-0.5f, 0.0f, 0.0f		// top left
	};
	unsigned int indicesSQARE[] = {	// note that we start from 0!
		 0, 1, 3,				// first triangle
		 1, 2, 3				// second triangle
	};
	unsigned int VAOsqare;
	glGenVertexArrays(1, &VAOsqare);
	glBindVertexArray(VAOsqare);
	unsigned int VBOsqare;
	glGenBuffers(1, &VBOsqare);
	glBindBuffer(GL_ARRAY_BUFFER, VBOsqare);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSQARE), verticesSQARE, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	unsigned int EBOsqare;
	glGenBuffers(1, &EBOsqare);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOsqare);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesSQARE), indicesSQARE, GL_STATIC_DRAW);

	glBindVertexArray(NULL);
	*/

	float verticesSQARE[] = {
		 0.5f, 0.8f, 0.0f,		// top right
		 0.5f, -0.8f, 0.0f,		// bottom right
		-0.5f, -0.8f, 0.0f,		// bottom left
		-0.5f, 0.8f, 0.0f		// top left
	};
	unsigned int indicesSQARE[] = {	// note that we start from 0!
		 0, 1, 3,				// first triangle
		 1, 2, 3				// second triangle
	};

	MeshObject quad(verticesSQARE, sizeof(verticesSQARE), indicesSQARE, sizeof(indicesSQARE));
	quad.setAtribute(0, 3);
	quad.instantiate();

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();


		float timeValue = (float)glfwGetTime() * 2;
		alphaValue = (sin(timeValue) / 2.0f) + 0.5f;
		shader.setUniformFloat("Color", 0.7f, 0.3f, 0.0f, alphaValue);
		quad.draw();
		
		/*
		glBindVertexArray(VAOtrig);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAOsqare);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		*/


		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	glfwPollEvents();
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		std::cout << "bye!" << std::endl;
	} else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		static bool isLines = false;
		if (isLines) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			isLines = !isLines;
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			isLines = !isLines;
		}
		
	}
}



#ifdef NDEBUG 
int WinMain(){ return main(); }
#endif