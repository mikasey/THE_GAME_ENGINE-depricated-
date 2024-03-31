//import std.compat;
#include <iostream>
#include <cmath>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "ShaderClass.h"

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

	/*
	const char* vertexShaderSource = "#version 330 core\n\
		layout (location = 0) in vec3 aPos;\n\
		void main(){ gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); }\0";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	const char* fragmentShaderSource = "#version 330 core\n\
		out vec4 FragColor;\n\
		uniform vec4 Color;\n\
		void main(){ FragColor = Color; }";
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glUseProgram(shaderProgram);
	*/

	Shader shader("assets//shader.vs", "assets//shader.fs");
	shader.use();
	float alphaValue = 1.0f;
	shader.setFloat4("Color", 0.7f, 0.3f, 0.0f, alphaValue);

	// int uniLoc = glGetUniformLocation(shaderProgram, "Color");
	// float alphaValue = 0.0f;
	// glUniform4f(uniLoc, 0.7f, 0.3f, 0.0f, alphaValue);

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

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();
		
		float timeValue = (float)glfwGetTime() * 2;
		alphaValue = (sin(timeValue) / 2.0f) + 0.5f;
		shader.setFloat4("Color", 0.7f, 0.3f, 0.0f, alphaValue);

		//glUseProgram(shaderProgram);
		// 
		// float timeValue = (float)glfwGetTime()*2;
		// float alphaValue = (sin(timeValue) / 2.0f) + 0.5f;
		// glUniform4f(uniLoc, 0.7f, 0.3f, 0.0f, alphaValue);

		glBindVertexArray(VAOtrig);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAOsqare);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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