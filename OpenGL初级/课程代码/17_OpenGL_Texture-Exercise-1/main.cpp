#include <iostream>

#include "glframework/core.h"
#include "glframework/shader.h"
#include <string>
#include <assert.h>//����
#include "wrapper/checkError.h"
#include "application/Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include "application/stb_image.h"
/*
*����������������������������������������������������������������������������������������������������
*����Ŀ	   �꣺ ������ϰ��һ��
*������    ʦ�� ������(Carma Zhao)
*�������Ŀ�꣺
*					-����ƽ��
*					-������(vsʵ�֡�fsʵ�֣����Ƽ���vsʵ��
*����������������������������������������������������������������������������������������������������
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
	//1 ׼��positions colors
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

	//2 VBO����
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

	//3 EBO����
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//4 VAO����
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//5 ��vbo ebo ��������������Ϣ
	//5.1 ����λ������������Ϣ
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.2 ������ɫ������������
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.3 ����uv������������
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	//5.4 ����ebo����ǰ��vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);
}

void prepareShader() {
	shader = new Shader("assets/shaders/vertex.glsl","assets/shaders/fragment.glsl");
}

void prepareTexture() {
	//1 stbImage ��ȡͼƬ
	int width, height, channels;

	//--��תy��
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("assets/textures/hinata.jpg", &width, &height, &channels, STBI_rgb_alpha);

	//2 ���������Ҽ��Ԫ��
	glGenTextures(1, &texture);
	//--��������Ԫ--
	glActiveTexture(GL_TEXTURE0);
	//--���������--
	glBindTexture(GL_TEXTURE_2D, texture);

	//3 ������������,�����Դ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//***�ͷ�����
	stbi_image_free(data);

	//4 ��������Ĺ��˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//5 ��������İ�����ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v
}

void render() {
	//ִ��opengl�����������
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	//�󶨵�ǰ��program
	shader->begin();

	shader->setInt("sampler", 0);
	shader->setFloat("time", glfwGetTime());

	//�󶨵�ǰ��vao
	GL_CALL(glBindVertexArray(vao));

	//��������ָ��
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

	//����opengl�ӿ��Լ�������ɫ
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