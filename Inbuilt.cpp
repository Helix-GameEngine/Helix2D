#include "h2dInbuilt.h"
#include "h2dBase.h"
#include "h2dImage.h"
#include <iostream>
#include <thread>
#include <mutex>
using namespace helix2d::Inbuilt;

std::vector<std::function<void()>> QueueMessage::msgPeek;

std::mutex QueueMessage::mtx_msgPeek;

Initialize Initialize::init;
bool Initialize::bGameDone = false;

Initialize::Initialize()
{
	auto hr = CoInitializeEx(
		NULL,
		COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE
	);

	while (!Renderer::createDeviceIndependentResources());

	std::thread thdResourcesCreater{ Initialize::RenderResourcesCreater };
	thdResourcesCreater.detach();
}

helix2d::Inbuilt::Initialize::~Initialize()
{
	bGameDone = true;
	Window::bAllWindowDone = true;

	//等待其它线程尽快关闭
	Sleep(1);

	//清空消息队列
	if (!QueueMessage::msgPeek.empty() &&
		QueueMessage::mtx_msgPeek.try_lock())
	{
		for (auto& msg : QueueMessage::msgPeek)
		{
			msg();
		}
		QueueMessage::msgPeek.clear();
		QueueMessage::mtx_msgPeek.unlock();
	}
	
	Window::uninit();
	Image::uninit();
}

void helix2d::Inbuilt::Initialize::RenderResourcesCreater()
{
	auto hr = CoInitializeEx(
		NULL,
		COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE
	);

	while (true)
	{
		if (bGameDone)
		{
			return;
		}

		if (!QueueMessage::msgPeek.empty()&&
			QueueMessage::mtx_msgPeek.try_lock())
		{
			auto& queue = QueueMessage::msgPeek;
			for (auto& msg : queue)
			{
				msg();
			}
			queue.clear();
			
			QueueMessage::mtx_msgPeek.unlock();
		}
	}
}


//void helix2d::Inbuilt::Initialize::GameManager()
//{
//	auto hr = CoInitializeEx(
//		NULL,
//		COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE
//	);
//
//	while (!Renderer::createDeviceIndependentResources());
//
//	while (true)
//	{
//		//printf("Game\n");
//		//创建设备相关资源
//		for (auto window : Window::allWindows)
//		{
//			if (window->bCreated)
//			{
//				auto b = window->getRenderer().createDeviceResources();
//			}
//		}
//
//		//if (!QueueMessage::msgPeek.empty() &&
//		//	QueueMessage::mtx_msgPeek.try_lock())
//		//{
//		//	for (size_t i = 0; i < QueueMessage::msgPeek.size(); )
//		//	{
//		//		auto it = QueueMessage::msgPeek.begin() + i;
//		//		(*it)();
//		//		QueueMessage::msgPeek.erase(it);
//		//	}
//		//	QueueMessage::mtx_msgPeek.unlock();
//		//}
//
//		/*if (!QueueMessage::msgPeek.empty() &&
//			QueueMessage::mtx_msgPeek.try_lock())
//		{
//			auto& queue = QueueMessage::msgPeek;
//			for (auto& msg : queue)
//			{
//				msg();
//			}
//			queue.erase(queue.begin(), queue.end());
//
//			QueueMessage::mtx_msgPeek.unlock();
//		}*/
//
//		//处理Expoint程序消息
//		if (!QueueMessage::msgPeek.empty() &&
//			QueueMessage::mtx_msgPeek.try_lock())
//		{
//			for (size_t i = 0; i < QueueMessage::msgPeek.size(); i++)
//			{
//				auto it = QueueMessage::msgPeek.begin() + i;
//				(*it)();
//				QueueMessage::msgPeek.erase(it);
//			}
//			QueueMessage::mtx_msgPeek.unlock();
//		}
//	}
//}
