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
*│　目	   标： 学习VAO相关操作
*│　讲    师： 赵新政(Carma Zhao)
*│　拆分目标：
*│
*│　		1  生成VAO，绑定VAO
*			2  使用SingleBuffer策略，进行VAO的构建
*			3  使用InterleavedBuffer策略，进行VAO的构建
*└────────────────────────────────────────────────┘
*/

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
}

void prepareSingleBuffer() {
	//1 准备positions colors数据
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


	//2 使用数据生成两个vbo posVbo， colorVbo
	GLuint posVbo, colorVbo;
	glGenBuffers(1, &posVbo);
	glGenBuffers(1, &colorVbo);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	//3 生成vao并且绑定
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//4 分别将位置/颜色属性的描述信息加入vao当中
	//4.1描述位置属性
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);//只有绑定了posVbo，下面的属性描述才会与此vbo相关
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//4.2 描述颜色属性
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
}


void prepareInterleavedBuffer() {
	//1 准备好Interleaved数据（位置+颜色）
	float vertices[] = {
		   -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.0f,  0.5f, 0.0f,  0.0f,  0.0f, 1.0f
	};


	//2 创建唯一的vbo
	GLuint vbo = 0;
	GL_CALL(glGenBuffers(1, &vbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	//3 创建并绑定vao
	GLuint vao = 0;
	GL_CALL(glGenVertexArrays(1, &vao));
	GL_CALL(glBindVertexArray(vao));


	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	//4 为vao加入位置和颜色的描述信息
	//4.1 位置描述信息
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));

	//4.2 颜色描述信息
	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));

	//5 扫尾工作：解绑当前vao
	glBindVertexArray(0);
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