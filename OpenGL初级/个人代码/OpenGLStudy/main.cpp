#include <iostream>

//ע�⣺gladͷ�ļ�������glfw����֮ǰ����
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h>//����
#include "wrapper/checkError.h"
#include "application/application.h"


//������ʵ��һ����Ӧ�����С�仯�ĺ���
void frameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
	//std::cout << "�������´�С��" << width << " , " << height << std::endl;
	//���ӿڴ�С�ʹ��ڴ�Сһ��ı�
	GL_CALL(glViewport(0, 0, width, height));
}

void OnResize(int width, int height){
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}


void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
}

void prepareSingleBuffer() {
	//1.׼��positions colors����
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	float colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	//2.ʹ��������������vbo  posVBO  colorVBO
	GLuint posVbo, colorVbo;
	GL_CALL(glGenBuffers(1, &posVbo));
	GL_CALL(glGenBuffers(1, &colorVbo));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW));

	//3.����vao���Ұ�
	GLuint vao = 0;
	GL_CALL(glGenVertexArrays(1, &vao));
	GL_CALL(glBindVertexArray(vao));

	//4.�ֱ�λ��/��ɫ���Ե�������Ϣ����vao����
	//4.1 ����λ������
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));//ֻ�а���posVbo ��������������Ż����vbo���
	GL_CALL(glEnableVertexAttribArray(0)); // ��λ�����Էŵ�0������λ��
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));

	//4.2������ɫ����
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVbo));//ֻ�а���posVbo ��������������Ż����vbo���
	GL_CALL(glEnableVertexAttribArray(1)); // ��λ�����Էŵ�1������λ��
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));

	GL_CALL(glBindVertexArray(0));//���
}

void prepareInterleavedBuffer() {
	//1 ׼����Interleaved����(λ��+��ɫ)
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	//2 ����Ψһ��vbo
	GLuint vbo = 0;
	GL_CALL(glGenBuffers(1, &vbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW));

	//3 ��������vao
	GLuint vao = 0;
	GL_CALL(glGenVertexArrays(1, &vao));
	GL_CALL(glBindVertexArray(vao));

	//4 Ϊvao����λ�ú���ɫ��������Ϣ
	//4.1 λ��������Ϣ
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));

	//4.2 ��ɫ������Ϣ
	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),(void* )(3 * sizeof(float))));

	//5 ɨβ����
	GL_CALL(glBindVertexArray(0));
}

/* ��VBO��ϰ
void prepareVBO() {
	//1.����һ��vbo,��û�����������Դ�!!����ֻ�Ǵ�����һ���������͵Ķ���
	GLuint vbo = 0;
	GL_CALL(glGenBuffers(1, &vbo));
	//2.����һ��vbo
	GL_CALL(glDeleteBuffers(1, &vbo));

	//3.����n��vbo
	GLuint vboArr[] = { 0,0,0 };
	GL_CALL(glGenBuffers(3, vboArr));

	//4.����n��vbo
	GL_CALL(glDeleteBuffers(3, vboArr));
}

void prepare() {
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
	};
	//1 ����һ��vbo
	GLuint vbo = 0;
	GL_CALL(glGenBuffers(1, &vbo));
	//2 �󶨵�ǰvbo,��opengl״̬���ĵ�ǰvbo�����
	//GL_ARRAY_BUFFER:��ʾ��ǰvbo������
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	//3 ��ǰvbo��������, Ҳ���ڿ����Դ�
	//���ݴ�CPU���ڴ浽��GPU���Դ�
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
}

void prepareSingleBuffer() {
	//1.׼������λ�����ݺ���ɫ����
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	float colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
	//2.Ϊλ��&��ɫ���ݸ�������һ��vbo
	GLuint posVbo = 0, colorVbo = 0;
	GL_CALL(glGenBuffers(1, &posVbo));
	GL_CALL(glGenBuffers(1, &colorVbo));

	//3.�������ֿ���vbo�����������
	//position�������
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));
	//color�������
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



*/

int main() {
	if (!app->init(800, 600))
	{
		return -1;
	}

	app->setResizeCallback(OnResize);
	app->setKeyBoardCallback(OnKey);

	//����opengl�ӿ��Լ�������ɫ
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	prepareSingleBuffer();

	while (app->update())
	{
		//ִ��opengl�����������
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	}

	app->destory();


	return 0;
}