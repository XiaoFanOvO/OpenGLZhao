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
*����Ŀ	   �꣺ ѧϰVAO��ز���
*������    ʦ�� ������(Carma Zhao)
*�������Ŀ�꣺
*��
*����		1  ����VAO����VAO
*			2  ʹ��SingleBuffer���ԣ�����VAO�Ĺ���
*			3  ʹ��InterleavedBuffer���ԣ�����VAO�Ĺ���
*����������������������������������������������������������������������������������������������������
*/

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
	GLuint vao = 0;
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

int main() {
	if (!app->init(800, 600)) {
		return -1;
	}

	app->setResizeCallback(OnResize);
	app->setKeyBoardCallback(OnKey);

	//����opengl�ӿ��Լ�������ɫ
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));


	prepareInterleavedBuffer();
	while (app->update()) {
		//ִ��opengl�����������
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
		//��Ⱦ����
	}

	app->destroy();

	return 0;
}