#include <iostream>

#define DEBUG

//ע�⣺gladͷ�ļ�������glfw����֮ǰ����
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h>//����
#include "wrapper/checkError.h"
#include "application/Application.h"

/*
*����������������������������������������������������������������������������������������������������
*����Ŀ	   �꣺ ѧϰglDrawArrays���л�������
*������    ʦ�� ������(Carma Zhao)
*�������Ŀ�꣺
*��
*����		1  ����GL_TRANGLES���л���
*				** �����ҿ������Ŵ��壬ʵ��NDC���������

*����		2  prepareVAOForGLTriangles:�����ĸ������vao
*����		3  ����Triangle������mode��TRIANGLES  TRIANGLE_STRIP TRIANGLE_FAN
*����		4  ����Line�Ķ���mode��LINES  LINE_STRIP
*����������������������������������������������������������������������������������������������������
*/

GLuint vao, program;


void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
}

void prepareSingleBuffer() {
	//1 ׼��positions colors����
	float positions[] = {
		   -0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
	};
	float colors[] = {
	   1.0f, 0.0f, 0.0f,
	   0.0f, 1.0f, 0.0f,
	   0.0f,  0.0f, 1.0f
	};


	//2 ʹ��������������vbo posVbo�� colorVbo
	GLuint posVbo, colorVbo;
	glGenBuffers(1, &posVbo);
	glGenBuffers(1, &colorVbo);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	//3 ����vao���Ұ�
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//4 �ֱ�λ��/��ɫ���Ե�������Ϣ����vao����
	//4.1����λ������
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);//ֻ�а���posVbo����������������Ż����vbo���
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//4.2 ������ɫ����
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
}


void prepareInterleavedBuffer() {
	//1 ׼����Interleaved���ݣ�λ��+��ɫ��
	float vertices[] = {
		   -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.0f,  0.5f, 0.0f,  0.0f,  0.0f, 1.0f
	};


	//2 ����Ψһ��vbo
	GLuint vbo = 0;
	GL_CALL(glGenBuffers(1, &vbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	//3 ��������vao
	GL_CALL(glGenVertexArrays(1, &vao));
	GL_CALL(glBindVertexArray(vao));


	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	//4 Ϊvao����λ�ú���ɫ��������Ϣ
	//4.1 λ��������Ϣ
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));

	//4.2 ��ɫ������Ϣ
	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));

	//5 ɨβ���������ǰvao
	glBindVertexArray(0);
}

void prepareVAOForGLTriangles() {
	//1 ׼��positions
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
		0.8f,  0.8f, 0.0f,
		0.8f,  0.0f, 0.0f
	};


	//2  posVbo
	GLuint posVbo;
	glGenBuffers(1, &posVbo);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	//3 ����vao���Ұ�
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//4 ����λ������
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
}

void prepareShader() {
	//1 ���vs��fs��Դ���룬����װ���ַ���
	const char* vertexShaderSource =
		"#version 460 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";


	//2 ����Shader����vs��fs��
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);


	//3 Ϊshader��������shader����
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

	int success = 0;
	char infoLog[1024];
	//4 ִ��shader������� 
	glCompileShader(vertex);
	//���vertex������
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
		std::cout << "Error: SHADER COMPILE ERROR --VERTEX" << "\n" << infoLog << std::endl;
	}

	glCompileShader(fragment);
	//���fragment������
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
		std::cout << "Error: SHADER COMPILE ERROR --FRAGMENT" << "\n" << infoLog << std::endl;
	}

	//5 ����һ��Program����
	program = glCreateProgram();

	//6 ��vs��fs����õĽ���ŵ�program���������
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	//7 ִ��program�����Ӳ������γ����տ�ִ��shader����
	glLinkProgram(program);

	//������Ӵ���
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 1024, NULL, infoLog);
		std::cout << "Error: SHADER LINK ERROR " << "\n" << infoLog << std::endl;
	}

	//����
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void render() {
	//ִ��opengl�����������
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	//1 �󶨵�ǰ��program
	glUseProgram(program);

	//2 �󶨵�ǰ��vao
	glBindVertexArray(vao);

	//3 ��������ָ��
	glDrawArrays(GL_LINE_STRIP, 0, 6);
}


int main() {
	if (!app->init(800, 600)) {
		return -1;
	}

	app->setResizeCallback(OnResize);
	app->setKeyBoardCallback(OnKey);

	//����opengl�ӿ��Լ�������ɫ
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	prepareShader();
	prepareVAOForGLTriangles();
	while (app->update()) {

		render();
		
	}

	app->destroy();

	return 0;
}