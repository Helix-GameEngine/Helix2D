#include "h2dInbuilt.h"
#include "h2dBase.h"
#include "h2dImage.h"
#include <iostream>
#include <thread>
#include <mutex>
using namespace helix2d::Inbuilt;

Initialize Initialize::init;
bool Initialize::bGameDone = false;

Initialize::Initialize()
{
	auto hr = CoInitializeEx(
		NULL,
		COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE
	);

	Renderer::createDeviceIndependentResources();
}

helix2d::Inbuilt::Initialize::~Initialize()
{
	bGameDone = true;
	Window::bAllWindowDone = true;

	//等待其它线程尽快关闭
	Sleep(1);
	
	Window::uninit();
	Image::uninit();
}