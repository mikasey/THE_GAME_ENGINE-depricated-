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
	isValid = true;
}
Shader::~Shader() {
	if (isValid) {
		glDeleteProgram(ID);
	}
}

Shader::Shader(const char* path) {
	// loading source strings
	std::string vertexString;
	std::string geometryString;
	std::string fragmentString;
	const char * vertexCString;
	const char * geometryCString;
	const char * fragmentCString;
	std::ifstream shaderFile;
	std::stringstream fileSStream;
	bool geometryDefined = false;

	try {
		shaderFile.open(path);
		fileSStream << shaderFile.rdbuf();
		shaderFile.close();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << e.what() << std::endl;
		return;
	}

	if (!fileSStream.str().contains("//# VERTEX FALSE")) {
		size_t first;
		size_t last;
		first = fileSStream.str().find("//# VERTEX START");
		last = fileSStream.str().find("//# VERTEX END");
		vertexString = fileSStream.str().substr(first, last - first).c_str();
	}
	else {
		vertexString = BASE_VERTEX_SHADER_STR_LIT;
	}
	if (!fileSStream.str().contains("//# GEOMETRY FALSE")) {
		size_t first;
		size_t last;
		first = fileSStream.str().find("//# GEOMETRY START");
		last = fileSStream.str().find("//# GEOMETRY END");
		geometryString = fileSStream.str().substr(first, last - first).c_str();

		geometryDefined = true;
	}
	else {
		geometryDefined = false;
	}
	if (!fileSStream.str().contains("//# FRAGMENT FALSE")) {
		size_t first;
		size_t last;
		first = fileSStream.str().find("//# FRAGMENT START");
		last = fileSStream.str().find("//# FRAGMENT END");
		fragmentString = fileSStream.str().substr(first, last - first).c_str();
	}
	else {
		fragmentString = BASE_FRAGMENT_SHADER_STR_LIT;
	}
	vertexCString = vertexString.c_str();
	geometryCString = geometryString.c_str();
	fragmentCString = fragmentString.c_str();

	// compiling shaders
	unsigned int vertexID, geometryID, fragmentID;
	int success;
	char infoLog[512];

	ID = glCreateProgram();
	vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID, 1, &vertexCString, NULL);
	glCompileShader(vertexID);
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fragmentCString, NULL);
	glCompileShader(fragmentID);
	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	geometryID = glCreateShader(GL_GEOMETRY_SHADER);
	if (geometryDefined) {
		glShaderSource(geometryID, 1, &geometryCString, NULL);
		glCompileShader(geometryID);
		glGetShaderiv(geometryID, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		glAttachShader(ID, geometryID);
	}
	glAttachShader(ID, vertexID);
	glAttachShader(ID, fragmentID);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	glDeleteShader(geometryID);
	isValid = true;
}

void Shader::use() const {
	glUseProgram(ID);
}
void Shader::setUniformBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setUniformInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setUniformFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setUniformFloat(const std::string& name, float value1, float value2) const {
	glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
}
void Shader::setUniformFloat(const std::string& name, float value1, float value2, float value3) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}
void Shader::setUniformFloat(const std::string& name, float value1, float value2, float value3, float value4) const {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}
void Shader::setUniformMat4(const std::string& name, const float *data) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, data);
}