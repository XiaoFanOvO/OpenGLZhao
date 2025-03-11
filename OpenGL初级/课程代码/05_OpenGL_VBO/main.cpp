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
*│　目	   标： 学习VBO相关操作
*│　讲    师： 赵新政(Carma Zhao)
*│　拆分目标：
*│
*│　		1  vbo 的创建/销毁（一个、一堆）
*│　		2  练习绑定vbo，练习向vbo当中传输数据
*				-glBindBuffer
*				-glBufferData
*│　		3  练习vbo与多属性数据的存储
*				-SingleBuffer：每一个属性放在一个单独的vbo当中
*				-InterleavedBuffer：讲所有属性都存储在一个vbo当中，并且数据是交叉的
*└────────────────────────────────────────────────┘
*/

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
}

void prepareVBO() {
	//1 创建一个vbo  *******还没有真正分配显存*********
	GLuint vbo = 0;
	GL_CALL(glGenBuffers(1, &vbo));

	//2 销毁一个vbo
	GL_CALL(glDeleteBuffers(1, &vbo));

	//3 创建n个vbo
	GLuint vboArr[] = {0, 0, 0};
	GL_CALL(glGenBuffers(3, vboArr));

	//4 销毁n个vbo
	GL_CALL(glDeleteBuffers(3, vboArr));
}

void prepare() {
	float vertices[] = {
		   -0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
	};

	//1 生成一个vbo
	GLuint vbo = 0;
	GL_CALL(glGenBuffers(1, &vbo));

	//2 绑定当前vbo，到opengl状态机的当前vbo插槽上
	//GL_ARRAY_BUFFER:表示当前vbo这个插槽
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

	//3 向当前vbo传输数据，也是在开辟显存
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
}

void prepareSingleBuffer() {
	//1 准备顶点位置数据与颜色数据
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

	//2 为位置&颜色数据各自生成一个vbo
	GLuint posVbo = 0, colorVbo = 0;
	GL_CALL(glGenBuffers(1, &posVbo));
	GL_CALL(glGenBuffers(1, &colorVbo));

	//3 给两个分开的vbo各自填充数据
	//position填充数据
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

	//color填充数据
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

	//设置opengl视口以及清理颜色
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	prepareInterleavedBuffer();

	while (app->update()) {
		//执行opengl画布清理操作
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
		//渲染操作
	}

	app->destroy();

	return 0;
}