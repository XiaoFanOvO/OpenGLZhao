#include <iostream>

//ע�⣺gladͷ�ļ�������glfw����֮ǰ����
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h>//����
#include "wrapper/checkError.h"
#include "application/application.h"


GLuint vao, program;

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
		   -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.0f,  0.5f, 0.0f,  0.0f,  0.0f, 1.0f
	};
	//2 ����Ψһ��vbo
	GLuint vbo = 0;
	GL_CALL(glGenBuffers(1, &vbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW));

	//3 ��������vao
	GL_CALL(glGenVertexArrays(1, &vao));
	GL_CALL(glBindVertexArray(vao));

	//4 Ϊvao����λ�ú���ɫ��������Ϣ
	//4.1 λ��������Ϣ
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));

	//4.2 ��ɫ������Ϣ
	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),(void* )(3 * sizeof(float))));

	//5 ɨβ����
	GL_CALL(glBindVertexArray(0));
}

void prepareVAOForGLTriangles() {
	//1 ׼��positions
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		 0.8f,  0.8f, 0.0f,
		 0.8f,  0.0f, 0.0f,
	};
	//2 ʹ����������posVbo
	GLuint posVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	//3 ����vao���Ұ�
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//4 ����λ������
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	GL_CALL(glBindVertexArray(0));

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
		0.0f, 0.0f,1.0f
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	//2 VBO����
	GLuint posVbo, colorVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	//3 EBO����
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//4 VAO����
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//5 ��vbo��ebo ��������������Ϣ
	//5.1 ����λ������������Ϣ
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);// ���������д��� �������ʡ�� ���ǲ�����
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	//5.2 ������ɫ������������
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.3 ����ebo����ǰ��vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // ���д�����Ѿ���ebo�󶨵��˵�ǰ��vao
	//���������д��� �������ﲻ��ʡ�� ��Ϊvbo��glVertexAttribPointer��������������е�ǰvbo�Ĳ�ѯ
	//eboû�����ִ��� ������glBindBuffer�Ļ�����������ȥ�鵱ǰ��û��ebo ������Ҫ��������
	glBindVertexArray(0);
}

void prepareShader() {
	//1 ���vs��fs��Դ���룬����װ���ַ���
	const char* vertexShaderSource =
		"#version 460 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 color;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"   color = aColor;\n"
		"}\0";
	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 color;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(color, 1.0f);\n"
		"}\n\0";

	//2 ����shader����(vs fs)
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	//3 Ϊshader��������shader����
	glShaderSource(vertex, 1, &vertexShaderSource, NULL); //��\0��Ȼ��β���ַ��� ����Ҫ����������ϵͳ�Լ���֪���������β
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

	int success = 0;
	char infoLog[1024];
	//4 ִ��shader�������
	glCompileShader(vertex);
	//���vertex������
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
		std::cout << "Error:SHADER COMPILE ERROR --VERTEX" << "\n" << infoLog << std::endl;
	}
	glCompileShader(fragment);
	//���fragment������
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
		std::cout << "Error:SHADER COMPILE ERROR --FRAGMENT" << "\n" << infoLog << std::endl;
	}

	//5 ����һ��Program����
	program = glCreateProgram();

	//6 ��vs��fs����õĽ���ŵ�program���������
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	//7 ִ��program�����Ӳ���,�γ����տ�ִ��shader����
	glLinkProgram(program);
	//������Ӵ���
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 1024, NULL, infoLog);
		std::cout << "Error:SHADER LINK ERROR" << "\n" << infoLog << std::endl;
	}

	//����
	glDeleteShader(vertex);//�Ѿ������program�� ��������ֱ�������
	glDeleteShader(fragment);

}

void render() {
	/*
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//���������һ�д��� ����Ч����ֻ����һ֡������ �ӵڶ�֡��ʼ�ͱ���
	//ԭ���������Ѿ�����ǰVAO����Ϊ��0 ��ǰVAO���滹��֮ǰ�󶨵�EBO����Ϣ 
	//�������VAO��ʱ�� ��ʱEBO����Ϣ���� (�������Ϊ���Զ�����Ϊ��ǰ��EBO) ���Ե�һ֡���Ի�����
	//���ǵڶ�֡ʱ,ͨ�������������ǰEBO����Ϊ0,��ʱ����VAO,����Ҳ�ὫVAO�ϰ󶨵�EBO����Ϊ0.�����ò������ݱ���
	*/
	//ִ��opengl�����������
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	//1 �󶨵�ǰ��program
	glUseProgram(program);
	//2 �󶨵�ǰ��vao
	glBindVertexArray(vao);
	//3 ��������ָ��
	//glDrawArrays(GL_TRIANGLES, 0, 6); // ���Զ���ÿ�����㹹��һ�������εķ�ʽ����Ⱦ
	//glDrawArrays(GL_LINE_STRIP, 0, 6); 
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(sizeof(int) * 3));//��������λ��ƫ��
	//unsigned int indices[] = {
	//	0, 1, 2,
	//	2, 1, 3
	//};
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);//��Ҫע������İ�ebo��vao�Ĵ���
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

	prepareShader();
	prepareVAO();

	while (app->update())
	{
		render();
	}

	app->destory();


	return 0;
}