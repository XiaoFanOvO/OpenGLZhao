#include <iostream>

//ע�⣺gladͷ�ļ�������glfw����֮ǰ����
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h>//����
#include "wrapper/checkError.h"

/*
*����������������������������������������������������������������������������������������������������
*����Ŀ	   �꣺ OpenGL������
*������    ʦ�� ������(Carma Zhao)
*�������Ŀ�꣺
*��
*����		1	����glGetError
*����		2	������������װΪ����
*����		3	�����������ŵ�������cpp�ļ�
*����		4	������GL_CALL����꣬��ݽ���gl�������
*����		5	��CMakeLists����ʹ��ȫ��Ԥ����꣬�������Ƿ���������
*����������������������������������������������������������������������������������������������������
*/

//������ʵ��һ����Ӧ�����С�仯�ĺ���
void frameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
//	std::cout << "�������´�С��" << width << " , " << height << std::endl;
	glViewport(0, 0, width, height);
}

//������ʵ��һ��������Ϣ�ص�����
void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_W) {
		//������w
	}
	if (action == GLFW_PRESS) {
		//��λ����
	}
	if (action == GLFW_RELEASE) {
		//��λ̧��
	}
	if (mods == GLFW_MOD_CONTROL) {
		//����ctrl��ͬʱ��������key
	}
	if (mods == GLFW_MOD_SHIFT) {
		//����shift��ͬʱ��������key
	}
	std::cout << "�����ˣ�" << key << std::endl;
	std::cout << "action��" << action << std::endl;
	std::cout << "mods��" << mods << std::endl;
}

int main() {
//1 ��ʼ��GLFW��������
	glfwInit();
	//1.1 ����OpenGL���汾�š��ΰ汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//1.2 ����OpenGL���ú���ģʽ����������Ⱦģʽ��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//2 �����������
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGLStudy", NULL, NULL);
	//**���õ�ǰ�������ΪOpenGL�Ļ�����̨
	glfwMakeContextCurrent(window);

	//���ü��������������С�仯����Ϣ
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
	//���ü���������������Ϣ
	glfwSetKeyCallback(window, keyCallBack);

//*****ʹ��glad�������е�ǰ�汾opengl�ĺ���
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//����opengl�ӿ��Լ�������ɫ
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

//3 ִ�д���ѭ��
	while (!glfwWindowShouldClose(window)) {
		//���ղ��ַ�������Ϣ
		//�����Ϣ�����Ƿ�����Ҫ�������ꡢ���̵���Ϣ
		//����еĻ��ͽ���Ϣ����������ն���
		glfwPollEvents();

		//ִ��opengl�����������
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
		
		
		//��Ⱦ����

		//�л�˫����
 		glfwSwapBuffers(window);
	}


//4 �˳�����ǰ���������
	glfwTerminate();

	return 0;
}