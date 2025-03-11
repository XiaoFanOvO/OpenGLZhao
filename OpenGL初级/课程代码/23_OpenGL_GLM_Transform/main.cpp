#include <iostream>

#include "glframework/core.h"
#include "glframework/shader.h"
#include <string>
#include <assert.h>//����
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/texture.h"

/*
*����������������������������������������������������������������������������������������������������
*����Ŀ	   �꣺ ����ѧϰGLM���ƽ��/��ת/����
*������    ʦ�� ������(Carma Zhao)
*�������Ŀ�꣺
*					-1 ����GLM��ͷ�ļ�
*					-2 ����vertexShader������uniform����mat4 transform
*					-3 ʹ�ô����transform�����ǵĶ���������б任
*					-4 Ϊshader������uniform������Ĺ���
*					-5 ʹ�þ��������ϱ任
*					-6 ����ת�������Σ�������
*����������������������������������������������������������������������������������������������������
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

void doRotationTransform() {
	//����һ����ת��������z����ת45�Ƚ�
	//rotate����������������ת����
	//bug1:rotate����õ�һ��float���͵ĽǶȣ�c++��template
	//bug2:rotate�������ܵĲ��ǽǶȣ�degree�������յĻ��ȣ�radians��
	//ע��㣺radians����Ҳ��ģ�庯�����м�Ҫ����float�������ݣ���f��׺
	transform = glm::rotate(glm::mat4(1.0f),glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
}

//ƽ�Ʊ任
void doTranslationTransform() {
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
}

//���ű任
void doScaleTransform() {
	transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.5f, 1.0f));
}

//���ϱ任
void doTransform() {
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
	//����ת ��ƽ��
	//transform = translateMat * rotateMat;

	//��ƽ�� ����ת
	transform = rotateMat * translateMat;
}

float angle = 0.0f;
void doRotation() {
	angle += 2.0f;
	//ÿһ֡���ᡰ���¡�����һ����ת����
	transform = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
}

void prepareVAO() {
	//1 ׼��positions colors
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
	texture = new Texture("assets/textures/goku.jpg", 0);
}

void render() {
	//ִ��opengl�����������
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	//�󶨵�ǰ��program
	shader->begin();
	shader->setInt("sampler", 0);
	shader->setMatrix4x4("transform", transform);

	//�󶨵�ǰ��vao
	GL_CALL(glBindVertexArray(vao));

	//��������ָ��
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

	//����opengl�ӿ��Լ�������ɫ
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	prepareShader();
	prepareVAO();
	prepareTexture();
	while (app->update()) {
		doRotation();
		render();
	}

	app->destroy();

	return 0;
}