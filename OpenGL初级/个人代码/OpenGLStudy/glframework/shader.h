#pragma once

#include "core.h"
#include <string>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void begin();//��ʼʹ�õ�ǰ��shader

	void end();//����ʹ�õ�ǰ��shader

	void setFloat(const std::string& name, float value);
	void setVector3(const std::string& name, float x, float y, float z);
	void setVector3(const std::string& name, const float* values);

	void setInt(const std::string& name, int value);
private:
	//��shader program�Ĵ�
	//type:COMPILE LINK
	void checkShaderErrors(GLuint target, std::string type);

private:
	GLuint mProgram{ 0 };
};
