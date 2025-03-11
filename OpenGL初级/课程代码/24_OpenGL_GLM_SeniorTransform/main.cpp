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
*│　目	   标： 学习GLM库叠加运算
*│　讲    师： 赵新政(Carma Zhao)
*│　拆分目标：
*					-1 连续转动三角形（动画）
*					-2 先平移再叠加旋转
*					-3 先旋转再叠加平移
*					-4 先缩放再叠加平移
*└────────────────────────────────────────────────┘
*/

GLuint vao;
Shader* shader = nullptr;
Texture* texture = nullptr;
glm::mat4 transform(1.0f);

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
}

void preTransform() {
	//目标二：先平移再叠加旋转
	//transform = glm::translate(transform, glm::vec3(0.6f, 0.0f, 0.0f));

	//目标三：先做一个旋转，只旋转一次
//	transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	//目标四：先做一个缩放，缩放只做一次
	transform = glm::scale(transform, glm::vec3(0.1f,1.0f,1.0f));
}

void doTransform() {
	//目标一：旋转的三角形
//	float angle = 1.0f;
//	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	
	//目标二：先平移再叠加旋转
	float angle = 1.0f;
//***	transform = glm::translate(transform, glm::vec3(0.003f, 0.0f, 0.0f));
//	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

	//目标三：先旋转再叠加平移
	//1 preTransform里面先做一个旋转
	//2 每一帧都做平移
	//transform = glm::translate(transform, glm::vec3(0.01f, 0.0f, 0.0f));

	//目标四：先做一次缩放，再叠加平移 
	//1 preTransform里面先做一个缩放
	//2 每一帧都做平移
	transform = glm::translate(transform, glm::vec3(0.01f, 0.0f, 0.0f));
}

void prepareVAO() {
	//1 准备positions colors
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
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

void render() {
	//执行opengl画布清理操作
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	//绑定当前的program
	shader->begin();
	shader->setInt("sampler", 0);
	shader->setMatrix4x4("transform", transform);

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

	preTransform();
	while (app->update()) {
		doTransform();
		render();
	}

	app->destroy();

	return 0;
}