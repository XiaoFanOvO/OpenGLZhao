#include <iostream>

//ע�⣺gladͷ�ļ�������glfw����֮ǰ����
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h>//����
#include "wrapper/checkError.h"
#include "application/application.h"


//������ʵ��һ����Ӧ�����С�仯�ĺ���
void frameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
	//std::cout << "�������´�С��" << width << " , " << height << std::endl;
	//���ӿڴ�С�ʹ��ڴ�Сһ��ı�
	GL_CALL(glViewport(0, 0, width, height));
}

void OnResize(int width, int height){
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}


void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
}

int main() {
	if (!app->init(800, 600))
	{
		return -1;
	}

	app->setResizeCallback(OnResize);
	app->setKeyBoardCallback(OnKey);

	//����opengl�ӿ��Լ�������ɫ
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	while (app->update())
	{
		//ִ��opengl�����������
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	}

	app->destory();


	return 0;
}