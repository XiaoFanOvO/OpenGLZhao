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
*│　目	   标： 学习glDrawArrays进行绘制命令
*│　讲    师： 赵新政(Carma Zhao)
*│　拆分目标：
*│
*│　		1  采用GL_TRANGLES进行绘制
*				** 这里大家可以缩放窗体，实验NDC坐标的作用

*│　		2  prepareVAOForGLTriangles:构建四个顶点的vao
*│　		3  测试Triangle的三种mode：TRIANGLES  TRIANGLE_STRIP TRIANGLE_FAN
*│　		4  测试Line的二种mode：LINES  LINE_STRIP
*└────────────────────────────────────────────────┘
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

void prepareVAOForGLTriangles() {
	//1 准备positions
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

	//3 生成vao并且绑定
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//4 描述位置属性
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
}

void prepareShader() {
	//1 完成vs与fs的源代码，并且装入字符串
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


	//2 创建Shader程序（vs、fs）
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);


	//3 为shader程序输入shader代码
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

	int success = 0;
	char infoLog[1024];
	//4 执行shader代码编译 
	glCompileShader(vertex);
	//检查vertex编译结果
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
		std::cout << "Error: SHADER COMPILE ERROR --VERTEX" << "\n" << infoLog << std::endl;
	}

	glCompileShader(fragment);
	//检查fragment编译结果
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
		std::cout << "Error: SHADER COMPILE ERROR --FRAGMENT" << "\n" << infoLog << std::endl;
	}

	//5 创建一个Program壳子
	program = glCreateProgram();

	//6 将vs与fs编译好的结果放到program这个壳子里
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	//7 执行program的链接操作，形成最终可执行shader程序
	glLinkProgram(program);

	//检查链接错误
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 1024, NULL, infoLog);
		std::cout << "Error: SHADER LINK ERROR " << "\n" << infoLog << std::endl;
	}

	//清理
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void render() {
	//执行opengl画布清理操作
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	//1 绑定当前的program
	glUseProgram(program);

	//2 绑定当前的vao
	glBindVertexArray(vao);

	//3 发出绘制指令
	glDrawArrays(GL_LINE_STRIP, 0, 6);
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

	prepareShader();
	prepareVAOForGLTriangles();
	while (app->update()) {

		render();
		
	}

	app->destroy();

	return 0;
}