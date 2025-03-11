#include <iostream>

#define DEBUG

//注意：glad头文件必须在glfw引用之前引用
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h>//断言
#include "wrapper/checkError.h"
#include "application/Application.h"

/*
*┌────────────────────────────────────────────────┐
*│　目	   标： OpenGL当中实现Application封装
*│　讲    师： 赵新政(Carma Zhao)
*│　拆分目标：
*│
*│　		1	编写一个单例类的Application
*│　		2	编写一个OnResize，响应窗体发生变化
*└────────────────────────────────────────────────┘
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

	//设置opengl视口以及清理颜色
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	while (app->update()) {
		//执行opengl画布清理操作
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
		//渲染操作
	}

	app->destroy();

	return 0;
}