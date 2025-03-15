#include <iostream>

#include "glframework/core.h"
#include "glframework/shader.h"
#include <string>
#include <assert.h>//����
#include "wrapper/checkError.h"
#include "application/application.h"

#define STB_IMAGE_IMPLEMENTATION 
#include "application/stb_image.h"


GLuint vao;
GLuint texture;
Shader* shader = nullptr;

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
	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2
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

	/*
	//��̬��ȡλ��
	//������shader����ʽдlocation = 0����ȡVAO������ID 
	//ֱ������������󶨵�ʱ�����glGetAttribLocation���ص�ID����
	GLuint posLocation = glGetAttribLocation(shader->mProgram, "aPos");
	GLuint colorLocation = glGetAttribLocation(shader->mProgram, "aColor");
	*/
	
	//5 ��vbo��ebo ��������������Ϣ
	//5.1 ����λ������������Ϣ
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);// ���������д��� �������ʡ�� ���ǲ�����
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // ���д�����Ѿ���ebo�󶨵��˵�ǰ��vao
	//���������д��� �������ﲻ��ʡ�� ��Ϊvbo��glVertexAttribPointer��������������е�ǰvbo�Ĳ�ѯ
	//eboû�����ִ��� ������glBindBuffer�Ļ�����������ȥ�鵱ǰ��û��ebo ������Ҫ��������
	glBindVertexArray(0);
}

void prepareShader() {
	shader = new Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
}

void prepareTexture() {
	//1 stbImage ��ȡͼƬ
	int width, height, channels;
	//--��תy��
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("assets/textures/goku.jpg", &width, &height, &channels, STBI_rgb_alpha);//��ȡ����ȫ��ת��ΪRGBA��ʽ
	//2 ���������Ҽ��Ԫ��
	glGenTextures(1, &texture);
	//--��������Ԫ--
	glActiveTexture(GL_TEXTURE0);
	//--���������--
	glBindTexture(GL_TEXTURE_2D, texture);//������Զ������漤���0������Ԫ����
	//3 ������������ �����Դ� ��CPU��GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//***�ͷ�����
	stbi_image_free(data);

	//4 ����������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // ��Ҫ������>ͼƬ���� ��˫���Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);// ��Ҫ������<ͼƬ���� ���ٽ�����

	//5 ��������İ�����ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v
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
	shader->begin();
	//shader->setFloat("time", glfwGetTime());//Ҫ����uniform����һ��Ҫ��useprogram
	//shader->setFloat("speed", 2.0f);//Ҫ����uniform����һ��Ҫ��useprogram
	//shader->setVector3("uColor", 0.3f, 0.4f, 0.5f);
	shader->setInt("sampler", 0);//����0������Ԫ��
	float color[] = {0.9f, 0.3f, 0.25f};
	shader->setVector3("uColor", color);
	//2 �󶨵�ǰ��vao
	glBindVertexArray(vao);
	//3 ��������ָ��
	//glDrawArrays(GL_TRIANGLES, 0, 6); // ���Զ���ÿ�����㹹��һ�������εķ�ʽ����Ⱦ
	//glDrawArrays(GL_LINE_STRIP, 0, 6); 
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	shader->end();
	//�������ֵ��÷�ʽ
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
	prepareTexture();

	while (app->update())
	{
		render();
	}

	app->destory();


	return 0;
}