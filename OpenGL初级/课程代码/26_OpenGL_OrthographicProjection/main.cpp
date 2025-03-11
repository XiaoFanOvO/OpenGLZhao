#include <iostream>

#include "glframework/core.h"
#include "glframework/shader.h"
#include <string>
#include <assert.h>//断言
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/texture.h"

/*
*┌────────────────────────────────────────────────┐
*│　目	   标： 学习使用正交投影矩阵
*│　讲    师： 赵新政(Carma Zhao)
*│　拆分目标：
*				-1 学会使用glm的ortho函数 （orthographic）
					***ortho的数据是摄像机坐标系下***
					1.1 使用glm的ortho函数，生成了一个正交投影矩阵	
						此矩阵的作用是：生成一个投影盒子，将内部顶点转化到NDC坐标系
					1.2 在vertexShader当中，添加了projectionMatrix的uniform变量	
					1.3 在每一帧渲染之前，更新projectionMatrix这个uniform
					
*				-2 学习使用非NDC数据
*					1 按照标准案例进行构建（ppt上）
*					2 将顶点向世界坐标系的-z方向推进较大范围（-5）(剪裁）
*					3 将可视范围盒子向相机坐标系的+x方向推进1个单位
*					4 保持可视范围盒子不动，动相机(1.0,0.0,0.1)(1.0,0.0,1.0-剪裁)
* 
*				-3 理解剪裁
*└────────────────────────────────────────────────┘
*/

GLuint vao;
Shader* shader = nullptr;
Texture* texture = nullptr;
glm::mat4 transform(1.0f);
glm::mat4 viewMatrix(1.0f);
glm::mat4 orthoMatrix(1.0f);

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
}

void prepareVAO() {
	//1 准备positions colors
	/*float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
	};*/
	float positions[] = {
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};


	float colors[] = {
		1.0f, 0.0f,0.0f,
		0.0f, 1.0f,0.0f,
		0.0f, 0.0f,1.0f,
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
	};

	//2 VBO创建
	GLuint posVbo, colorVbo, uvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	//3 EBO创建
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//4 VAO创建
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//5 绑定vbo ebo 加入属性描述信息
	//5.1 加入位置属性描述信息
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.2 加入颜色属性描述数据
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.3 加入uv属性描述数据
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	//5.4 加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);
}

void prepareShader() {
	shader = new Shader("assets/shaders/vertex.glsl","assets/shaders/fragment.glsl");
}

void prepareTexture() {
	texture = new Texture("assets/textures/goku.jpg", 0);
}

void prepareCamera() {
	//lookat:生成一个viewMatrix
	//eye:当前摄像机所在的位置
	//center:当前摄像机看向的那个点
	//up:穹顶向量
	viewMatrix = glm::lookAt(glm::vec3(1.0f,0.0f,1.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
}

void prepareOrtho() {
	orthoMatrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 2.0f, -2.0f);
}

void render() {
	//执行opengl画布清理操作
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	//绑定当前的program
	shader->begin();
	shader->setInt("sampler", 0);
	shader->setMatrix4x4("transform", transform);
	shader->setMatrix4x4("viewMatrix", viewMatrix);
	shader->setMatrix4x4("projectionMatrix", orthoMatrix);
 
	//绑定当前的vao
	GL_CALL(glBindVertexArray(vao));

	//发出绘制指令
	GL_CALL(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
	GL_CALL(glBindVertexArray(0));

	shader->end();
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
	prepareVAO();
	prepareTexture();
	prepareCamera();
	prepareOrtho();

	while (app->update()) {
		render();
	}

	app->destroy();

	return 0;
}