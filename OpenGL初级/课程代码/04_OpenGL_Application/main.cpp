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
*����Ŀ	   �꣺ OpenGL����ʵ��Application��װ
*������    ʦ�� ������(Carma Zhao)
*�������Ŀ�꣺
*��
*����		1	��дһ���������Application
*����		2	��дһ��OnResize����Ӧ���巢���仯
*����������������������������������������������������������������������������������������������������
*/

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
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

	while (app->update()) {
		//ִ��opengl�����������
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
		//��Ⱦ����
	}

	app->destroy();

	return 0;
}