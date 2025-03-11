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
*����Ŀ	   �꣺ ѧϰVBO��ز���
*������    ʦ�� ������(Carma Zhao)
*�������Ŀ�꣺
*��
*����		1  vbo �Ĵ���/���٣�һ����һ�ѣ�
*����		2  ��ϰ��vbo����ϰ��vbo���д�������
*				-glBindBuffer
*				-glBufferData
*����		3  ��ϰvbo����������ݵĴ洢
*				-SingleBuffer��ÿһ�����Է���һ��������vbo����
*				-InterleavedBuffer�����������Զ��洢��һ��vbo���У����������ǽ����
*����������������������������������������������������������������������������������������������������
*/

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
}

void prepareVBO() {
	//1 ����һ��vbo  *******��û�����������Դ�*********
	GLuint vbo = 0;
	GL_CALL(glGenBuffers(1, &vbo));

	//2 ����һ��vbo
	GL_CALL(glDeleteBuffers(1, &vbo));

	//3 ����n��vbo
	GLuint vboArr[] = {0, 0, 0};
	GL_CALL(glGenBuffers(3, vboArr));

	//4 ����n��vbo
	GL_CALL(glDeleteBuffers(3, vboArr));
}

void prepare() {
	float vertices[] = {
		   -0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
	};

	//1 ����һ��vbo
	GLuint vbo = 0;
	GL_CALL(glGenBuffers(1, &vbo));

	//2 �󶨵�ǰvbo����opengl״̬���ĵ�ǰvbo�����
	//GL_ARRAY_BUFFER:��ʾ��ǰvbo������
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

	//3 ��ǰvbo�������ݣ�Ҳ���ڿ����Դ�
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
}

void prepareSingleBuffer() {
	//1 ׼������λ����������ɫ����
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

	//2 Ϊλ��&��ɫ���ݸ�������һ��vbo
	GLuint posVbo = 0, colorVbo = 0;
	GL_CALL(glGenBuffers(1, &posVbo));
	GL_CALL(glGenBuffers(1, &colorVbo));

	//3 �������ֿ���vbo�����������
	//position�������
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

	//color�������
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW));
}

void prepareInterleavedBuffer() {
	float vertices[] = {
		   -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	GLuint vbo = 0;
	GL_CALL(glGenBuffers(1, &vbo));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

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