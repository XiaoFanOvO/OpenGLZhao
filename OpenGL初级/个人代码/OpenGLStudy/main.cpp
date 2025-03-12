#include <iostream>

//注意：glad头文件必须在glfw引用之前引用
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h>//断言
#include "wrapper/checkError.h"
#include "application/application.h"


//声明且实现一个响应窗体大小变化的函数
void frameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
	//std::cout << "窗体最新大小：" << width << " , " << height << std::endl;
	//让视口大小和窗口大小一起改变
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

	//设置opengl视口以及清理颜色
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	while (app->update())
	{
		//执行opengl画布清理操作
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	}

	app->destory();


	return 0;
}