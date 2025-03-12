#pragma once
#include <iostream>
//#include <GLFW/glfw3.h> ����include ��Ϊmain�����Ѿ�����

#define app Application::getInstance()
class GLFWwindow; //����һ��class��Ŀ���Ƿ�ֹ���뱨�� ������Զ��滻��(cpp�������include)

using ResizeCallback = void(*)(int width, int height);
using KeyBoardCallback = void(*)(int key, int action, int mods);

class Application
{
public:
	~Application();
	//���ڷ���ʵ���ľ�̬����
	static Application* getInstance();

	//��������ͷ���false ����true
	bool init(const int& width = 800, const int& height = 600); 
	bool update();
	void destory();
	
	uint32_t getWidth() const { return mWidth; }
	uint32_t getHeight() const { return mHeight; }

	void setResizeCallback(ResizeCallback callback) { mResizeCallback = callback; }
	void setKeyBoardCallback(KeyBoardCallback callback) { mKeyBoardCallback = callback; }

private:
	//C++���ں���ָ��
	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
	//������ʵ��һ��������Ϣ�ص�����
	//key:��ĸ������
	//scancode:��������
	//action:���»���̧��
	//mods:�Ƿ���shift��ctrl
	static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	//ȫ��Ψһ�ľ�̬����ʵ��
	static Application* mInstance;

	uint32_t mWidth{ 0 };
	uint32_t mHeight{ 0 };
	GLFWwindow* mWindow{ nullptr };

	ResizeCallback mResizeCallback{nullptr};
	KeyBoardCallback mKeyBoardCallback{ nullptr };


	Application();  
};
