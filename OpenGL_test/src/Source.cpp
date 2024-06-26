#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb/stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ShaderClass.h"
#include "MeshObjectClass.h"

const char* winName = "NYA CAWAI!!!";
unsigned int winWidth = 600;
unsigned int winHeight = 600;
unsigned int winFPS = 12;
GLFWwindow* window = NULL;
unsigned int texture;
unsigned int texHSize = 12;
unsigned int texVSize = 1;


void processInput(GLFWwindow*);
int initEngine();
int loadTexture(const char* name);

int main() {
	if (int err = initEngine()) return err;
	
	Shader shader("assets//test.gls");
	shader.use();
	shader.setUniformFloat("animeSize", (float)texHSize);

	loadTexture("assets/textures.png");

	float verticesSQARE[] = {
		 1.0f,	1.0f, 0.0f,	1.0f, 1.0f,	// top right
		 1.0f, -1.0f, 0.0f,	1.0f, 0.0f,	// bottom right
		-1.0f, -1.0f, 0.0f,	0.0f, 0.0f,	// bottom left
		-1.0f,  1.0f, 0.0f,	0.0f, 1.0f,	// top left
	};
	unsigned int indicesSQARE[] = {	// note that we start from 0!
		 0, 1, 3,				// first triangle
		 1, 2, 3				// second triangle
	};
	MeshObject quad(verticesSQARE, sizeof(verticesSQARE), indicesSQARE, sizeof(indicesSQARE));
	quad.setAtribute(0, 3);
	quad.setAtribute(1, 2);
	quad.instantiate();

	float verticesCUBE[]{
		 1.0f,	1.0f, -1.0f,	1.0f, 1.0f,	// top right front		0
		 1.0f, -1.0f, -1.0f,	1.0f, 0.0f,	// bottom right front	1
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,	// bottom left front	2
		-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,	// top left front		3
																
		 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,	// bottom right back	5
		-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,	// bottom left back		6
		-1.0f,  1.0f,  1.0f,	0.0f, 1.0f,	// top left back		7
		 1.0f,	1.0f,  1.0f,	1.0f, 1.0f,	// top right back		4
	};
	unsigned int indicesCUBE[]{
		0, 1, 3,							// front
		1, 2, 3,							//
		4, 5, 7,							// back
		5, 6, 7,							//
											
		//0, 1, 5,							// left
		//0, 4, 5,							//
		//2, 3, 7,							// roght
		//2, 6, 7,							//
		//									
		//1, 2, 6,							// top
		//1, 5, 6,							//
		//0, 3, 7,							// down
		//0, 4, 7								//

	};
	MeshObject cube(verticesCUBE, sizeof(verticesCUBE), indicesCUBE, sizeof(indicesCUBE));
	cube.setAtribute(0, 3);
	cube.setAtribute(1, 2);
	cube.instantiate();
	cube.scale = glm::vec3(0.3f);
	cube.rotation = glm::vec3(-45);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	shader.setUniformFloat("tilesetSize", 32.0f,32.0f);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();

		//static int frame = 1;
		//if (frame > 12) frame = 1;
		//shader.setUniformInt("frame", frame);
		//frame++;

		cube.draw(shader);

		glfwSwapBuffers(window);
		std::this_thread::sleep_for(1000ms/winFPS);
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

int initEngine()
{
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(winWidth, winHeight, winName, NULL, NULL);
	if (window == NULL) {
		std::cerr << "Window not go brrrrr\n" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Glad has failed you...\n" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, winWidth, winHeight);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });

	return 0;
}

int loadTexture(const char* name)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imgData = stbi_load(name, &width, &height, &nrChannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imgData);
	glBindTexture(GL_TEXTURE_2D, texture);

	return 0;
}



#ifdef NDEBUG 
int WinMain(){ return main(); }
#endif