#include "application.h"


//��ʼ��Application�ľ�̬����
Application* Application::mInstance = nullptr;
Application* Application::getInstance() {
	//���mInstance�Ѿ�ʵ������(new������)  ��ֱ�ӷ���
	//������Ҫ��new����,�ٷ���
	if (mInstance == nullptr)
	{
		mInstance = new Application();
	}
	return mInstance;
}


Application::Application() {

}

Application::~Application() {

}