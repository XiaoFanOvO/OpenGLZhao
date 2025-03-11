#include <iostream>

//注意：glad头文件必须在glfw引用之前引用
#include<glad/glad.h>
#include <GLFW/glfw3.h>

/*
* 目标：
	-创建glfw的窗体系统
	-加入窗体变化的事件回调
	-加入键盘消息事件回调
* 
*/

//声明且实现一个响应窗体大小变化的函数
void frameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
	std::cout << "窗体最新大小：" << width << " , " << height << std::endl;
}

//声明且实现一个键盘消息回调函数
void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_W) {
		//触发了w
	}
	if (action == GLFW_PRESS) {
		//键位按下
	}
	if (action == GLFW_RELEASE) {
		//键位抬起
	}
	if (mods == GLFW_MOD_CONTROL) {
		//按下ctrl的同时，按下了key
	}
	if (mods == GLFW_MOD_SHIFT) {
		//按下shift的同时，按下了key
	}
	std::cout << "按下了：" << key << std::endl;
	std::cout << "action：" << action << std::endl;
	std::cout << "mods：" << mods << std::endl;
}

int main() {
//1 初始化GLFW基本环境
	glfwInit();
	//1.1 设置OpenGL主版本号、次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//1.2 设置OpenGL启用核心模式（非立即渲染模式）
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//2 创建窗体对象
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGLStudy", NULL, NULL);
	//**设置当前窗体对象为OpenGL的绘制舞台
	glfwMakeContextCurrent(window);

	//设置监听，监听窗体大小变化的消息
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
	//设置监听，监听键盘消息
	glfwSetKeyCallback(window, keyCallBack);


//3 执行窗体循环
	while (!glfwWindowShouldClose(window)) {
		//接收并分发窗体消息
		//检查消息队列是否有需要处理的鼠标、键盘等消息
		//如果有的话就将消息批量处理，清空队列
		glfwPollEvents();
	}


//4 退出程序前做相关清理
	glfwTerminate();

	return 0;
}