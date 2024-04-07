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
#include "MeshObject.h"

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

	loadTexture("assets/chibi_girl.png");

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
	
	
	float angle = 0 ;
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.0, 0.0, 0.0));
	trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	trans = glm::scale(trans, glm::vec3(1.0, 1.0, 1.0));
	shader.setUniformMat4("objTrans", glm::value_ptr(trans));
	

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT);


		shader.use();

		static int frame = 1;
		if (frame > 12) frame = 1;
		shader.setUniformInt("frame", frame);
		frame++;
		quad.draw();

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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