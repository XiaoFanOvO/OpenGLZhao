#include "shader.h"
#include "../wrapper/checkError.h"
#include <string>
#include <fstream>
#include <sstream>	
#include <iostream>


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	//���յ�Ŀ���ǰ�������shader��program���� ����ֱ��ͨ��mprogram����
	//����װ��shader�ַ���������string
	std::string vertexCode;
	std::string fragmentCode;

	//�������ڶ�ȡvs��fs�ļ���inFileStream
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	
	//��֤ifstream��������ʱ�����׳��쳣
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit); //�ļ���ȡʧ�ܺ��ļ���
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit); //�ļ���ȡʧ�ܺ��ļ���

	try
	{
		//1 ���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		//2 ���ļ����������е��ַ������뵽stringStream����
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();  //���ļ��������ж����ַ��� �����뵽vShaderStream���������
		fShaderStream << fShaderFile.rdbuf(); 

		//3 �ر��ļ�
		vShaderFile.close();
		fShaderFile.close();

		//4 ���ַ�����stringStream���ж�ȡ���� ת����code String����
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR: SHADER File Error: " << e.what() << std::endl;
	}

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();
	//1 ����shader����(vs fs)
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	//2 Ϊshader��������shader����
	glShaderSource(vertex, 1, &vertexShaderSource, NULL); //��\0��Ȼ��β���ַ��� ����Ҫ����������ϵͳ�Լ���֪���������β
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

	//3 ִ��shader�������
	glCompileShader(vertex);
	checkShaderErrors(vertex, "COMPILE");

	glCompileShader(fragment);
	checkShaderErrors(fragment, "COMPILE");
	//4 ����һ��Program����
	mProgram = glCreateProgram();

	//5 ��vs��fs����õĽ���ŵ�program���������
	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);

	//6 ִ��program�����Ӳ���,�γ����տ�ִ��shader����
	glLinkProgram(mProgram);
	//������Ӵ���
	checkShaderErrors(mProgram, "LINK");
	//����
	glDeleteShader(vertex);//�Ѿ������program�� ��������ֱ�������
	glDeleteShader(fragment);
}

Shader::~Shader() {}

void Shader::begin() {
	GL_CALL(glUseProgram(mProgram));
}

void Shader::end() {
	GL_CALL(glUseProgram(0));
}

//�����޸�uniform����֮ǰһ��Ҫ��useprogram!
void Shader::setFloat(const std::string& name, float value) {
	//1 ͨ�������õ�Uniform������λ��Location
	GLuint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
	//����������ҵķ�Χ������program ������vs����fs ֻҪ�ж��� 
	//��������Ļ��������ᷢ������ �൱��һ�� 
	//���������Ҫͬʱ�޸�vs��fs���ͬ��uniform���� ֻ��Ҫ����һ�μ���

	//2 ͨ��Location����Uniform������ֵ
	GL_CALL(glUniform1f(location, value));
}

void Shader::setVector3(const std::string& name, float x, float y, float z) {
	//1 ͨ�������õ�Uniform������λ��Location
	GLuint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
	//2 ͨ��Location����Uniform������ֵ
	glUniform3f(location, x, y, z);
}

//����
void Shader::setVector3(const std::string& name, const float* values) {
	//1 ͨ�������õ�Uniform������λ��Location
	GLuint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
	//2 ͨ��Location����Uniform������ֵ
	//�ڶ�������:��ǰҪ���µ�uniform�������������,������������˶��ٸ�����vec3
	glUniform3fv(location, 1, values);
}


void Shader::setInt(const std::string& name, int value) {
	//1 ͨ�������õ�Uniform������λ��Location
	GLuint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
	//2 ͨ��Location����Uniform������ֵ
	//�ڶ�������:��ǰҪ���µ�uniform�������������,������������˶��ٸ�����vec3
	glUniform1i(location, value);

}

void Shader::checkShaderErrors(GLuint target, std::string type) {
	int success = 0;
	char infoLog[1024];
	if (type == "COMPILE")
	{
		glGetShaderiv(target, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(target, 1024, NULL, infoLog);
			std::cout << "Error:SHADER COMPILE ERROR" << "\n" << infoLog << std::endl;
		}
	}
	else if (type == "LINK")
	{
		glGetProgramiv(target, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(target, 1024, NULL, infoLog);
			std::cout << "Error:SHADER LINK ERROR" << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		std::cout << "Error: Check shader errors Type is wrong" << std::endl;
	}
}

