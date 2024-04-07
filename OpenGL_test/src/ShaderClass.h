#pragma once

#include <glad/glad.h>
#include "glm/glm.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int ID;
	bool isValid = false;

	Shader(const char* vertPath, const char* fragPath);
	~Shader();
	Shader(const char* path);
	
	void use() const;
	void setUniformBool(const std::string& name, bool value) const;
	void setUniformInt(const std::string& name, int value) const;
	void setUniformFloat(const std::string& name, float value) const;
	void setUniformFloat(const std::string& name, float value1, float value2) const;
	void setUniformFloat(const std::string& name, float value1, float value2, float value3) const;
	void setUniformFloat(const std::string& name, float value1, float value2, float value3, float value4) const;
	void setUniformMat4(const std::string& name, const float* data) const;


};

#define BASE_VERTEX_SHADER_STR_LIT     "#version 330 core\n\
										layout (location = 0) in vec3 aPos;\n\
										void main(){\n\
											gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
										}\0"
#define BASE_GEOMETRY_SHADER_STR_LIT   "#version 330 core\n\
										layout(triangles) in;\n\
										layout(triangle_strip, max_vertices = 3) out;\n\
										void main() {\n\
											for (int i = 0; i < 3; i++) {\n\
												gl_Position = gl_in[i].gl_Position;\n\
												EmitVertex();\n\
											}\n\
											EndPrimitive();\n\
										}"
#define BASE_FRAGMENT_SHADER_STR_LIT   "#version 330 core\n\
										out vec4 FragColor;\n\
										uniform vec4 Color;\n\
										void main(){\n\
											FragColor = Color;\n\
										}"
