#include "ShaderClass.h"

Shader::Shader(const char* vertPath, const char* fragPath) {
	std::string vertCode, fragCode;
	std::ifstream vertShaderFile, fragShaderFile;
	vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	
	try {
		vertShaderFile.open(vertPath);
		fragShaderFile.open(fragPath);
		std::stringstream vertShaderStream, fragShaderStream;
		vertShaderStream << vertShaderFile.rdbuf();
		fragShaderStream << fragShaderFile.rdbuf();
		vertShaderFile.close();
		fragShaderFile.close();
		vertCode = vertShaderStream.str();
		fragCode = fragShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << e.what() << std::endl;
	}
	const char* vertShaderCode = vertCode.c_str();
	const char* fragShaderCode = fragCode.c_str();

	unsigned int vertexID, fragmentID;
	int success;
	char infoLog[512];

	vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID,1,&vertShaderCode, NULL);
	glCompileShader(vertexID);
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fragShaderCode, NULL);
	glCompileShader(fragmentID);
	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexID);
	glAttachShader(ID, fragmentID);
	glLinkProgram(ID);
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// delete shaders; they’re linked into our program and no longer necessary
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

void Shader::use() const {
	glUseProgram(ID);
}
void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat2(const std::string& name, float value1, float value2) const {
	glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
}
void Shader::setFloat3(const std::string& name, float value1, float value2, float value3) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}
void Shader::setFloat4(const std::string& name, float value1, float value2, float value3, float value4) const {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}