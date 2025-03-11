#pragma once

#include "core.h"
#include<string>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	
	void begin();//��ʼʹ�õ�ǰShader

	void end();//����ʹ�õ�ǰShader

	void setFloat(const std::string& name, float value);

private:
	//shader program
	//type:COMPILE LINK
	void checkShaderErrors(GLuint target,std::string type);

private:
	GLuint mProgram{ 0 };
};
