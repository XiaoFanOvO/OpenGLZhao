#include <iostream>

#include "glframework/core.h"
#include "glframework/shader.h"
#include <string>
#include <assert.h>//断言
#include "wrapper/checkError.h"
#include "application/Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include "application/stb_image.h"
/*
*┌────────────────────────────────────────────────┐
*│　目	   标： 纹理练习（一）
*│　讲    师： 赵新政(Carma Zhao)
*│　拆分目标：
*					-纹理平铺
*					-纹理动画(vs实现、fs实现），推荐在vs实现
*└────────────────────────────────────────────────┘
*/

GLuint vao;
GLuint texture;
Shader* shader = nullptr;

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
}



void prepareVAO() {
	//1 准备positions colors
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};

	float colors[] = {
		1.0f, 0.0f,0.0f,
		0.0f, 1.0f,0.0f,
		0.0f, 0.0f,1.0f,
		0.5f, 0.5f,0.5f
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 1, 3
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
	//1 stbImage 读取图片
	int width, height, channels;

	//--反转y轴
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("assets/textures/hinata.jpg", &width, &height, &channels, STBI_rgb_alpha);

	//2 生成纹理并且激活单元绑定
	glGenTextures(1, &texture);
	//--激活纹理单元--
	glActiveTexture(GL_TEXTURE0);
	//--绑定纹理对象--
	glBindTexture(GL_TEXTURE_2D, texture);

	//3 传输纹理数据,开辟显存
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//***释放数据
	stbi_image_free(data);

	//4 设置纹理的过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//5 设置纹理的包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v
}

void render() {
	//执行opengl画布清理操作
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	//绑定当前的program
	shader->begin();

	shader->setInt("sampler", 0);
	shader->setFloat("time", glfwGetTime());

	//绑定当前的vao
	GL_CALL(glBindVertexArray(vao));

	//发出绘制指令
	GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
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

	while (app->update()) {

		render();
		
	}

	app->destroy();

	return 0;
}