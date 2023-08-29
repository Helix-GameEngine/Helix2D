#include "h2dBase.h"
#include "h2dCamera.h"
using namespace helix2d::Inbuilt;
#include <thread>
#include <chrono>
using namespace std::chrono;
#include <VersionHelpers.h>

/******************************************/

size_t helix2d::Window::ordinal{ 0 };

bool helix2d::Window::bAllWindowDone = false;

std::vector<helix2d::Window*> helix2d::Window::allWindows;

helix2d::Window::Window(
	std::wstring title,
	unsigned int width, unsigned int height,
	Window* parent,
	unsigned int fps
)
{
	//保证Initialize文件内容被包含进lib
	Initialize::function();

	{
		wc = WNDCLASSEXW();
		hWnd = HWND();
		
		this->title = title;

		this->width = width;
		this->height = height;

		this->parent = parent;

		this->fps = fps;

		renderer = new Renderer(this);

		camera = nullptr;

		bCreated = false;
		bFullScreen = false;
		bWindowDone = false;
	}

	allWindows.push_back(this);

	std::thread winThread{ &Window::winControl ,this };
	winThread.detach();
}

helix2d::Window::~Window()
{
	PostMessageW(hWnd, WM_CLOSE, 0, 0);

	bool bExit{ false };
	while (true)
	{
		if (allWindows.size() == 1)
		{
			bExit = true;
		}
		break;
	}
	if (bExit)
	{
		exit(0);
		return;
	}

	bCreated = false;

	bWindowDone = true;

	hWnd = HWND();
	wc = WNDCLASSEXW();

	for (auto& painter : painterList)
	{
		painter->setWindow(nullptr);
	}

	painterList.clear();
	clearInput();
	delete renderer;

	UnregisterClassW(wc.lpszClassName,
		HINST_THISCOMPONENT);

	allWindows.erase(
		std::find(
			allWindows.begin(), allWindows.end(), this)
	);
}

void helix2d::Window::addPainter(Painter* pPainter)
{
	if (pPainter == nullptr)
	{
		Logger::warning(L"The added Painter is nullptr. The Painter will not be added.");
		return;
	}

	if (pPainter->parent || pPainter->window)
	{
		Logger::warning(L"The added Painter has been added under any Painter or Window. The Painter will not be added.");
		return;
	}

	if (!pPainter->name.empty())
	{
		for (size_t i = 0; i < painterList.size(); i++)
		{
			auto p = painterList[i];
			if (p->name == pPainter->name)
			{
				Logger::warning(L"The name of the Painter has been repeated. The Painter will not be added.");
				return;
			}
		}
	}

	pPainter->setWindow(this);
	painterList.push_back(pPainter);
}

void helix2d::Window::addPainter(Painter* pPainter, std::wstring name)
{
	pPainter->setName(name);
	addPainter(pPainter);
}

bool helix2d::Window::removePainter(Painter* pPainter)
{
	if (SafeRemove(painterList, pPainter))
	{
		pPainter->parent = nullptr;
		pPainter->setWindow(nullptr);
		return true;
	}
	return false;
}

helix2d::Painter* helix2d::Window::findPainter(std::wstring name)
{
	//空名字（默认名字）则不查找
	if (name.empty())
	{
		return nullptr;
	}

	//开始查找Painter
	auto it = std::find_if(painterList.begin(), painterList.end(),
		[name](Painter* painter) {
			return painter->getName() == name;
		}
	);

	//检测是否有该Painter
	if (it == painterList.end())
	{
		return nullptr;
	}

	return (*it);
}

void helix2d::Window::setBackgroundColor(const Color& c)
{
	backgroundColor = c;
}

void helix2d::Window::setFPS(unsigned int fps)
{
	this->fps = fps;
}

void helix2d::Window::setCamera(Camera* camera)
{
	Painter* p = camera;

	if (p == nullptr)
	{
		this->camera = nullptr;
		return;
	}

	if (p->window == this)
	{
		this->camera = camera;
	}

	return;
}

void helix2d::Window::setFullScreen(bool fullScreen)
{
	bFullScreen = fullScreen;
}

unsigned int helix2d::Window::getWidth() const
{
	return width;
}

unsigned int helix2d::Window::getHeight() const
{
	return height;
}

HWND helix2d::Window::getHWND() const
{
	return hWnd;
}

helix2d::Renderer* helix2d::Window::getRenderer() const
{
	if (bCreated)
	{
		return renderer;
	}
	return nullptr;
}

std::vector<helix2d::Painter*> helix2d::Window::getAllPainter() const
{
	return painterList;
}

helix2d::Camera* helix2d::Window::getCamera() const
{
	return camera;
}

void helix2d::Window::uninit()
{
	for (auto& pWin : allWindows)
	{
		pWin->renderer->discardDeviceResources();
		if (pWin->bCreated)
		{
			PostMessageW(pWin->hWnd, WM_CLOSE, 0, 0);
			UnregisterClassW(pWin->wc.lpszClassName, 
				HINST_THISCOMPONENT);
			pWin->bCreated = false;
		}
	}
	allWindows.clear();

	Renderer::discardResources();
}

void helix2d::Window::create()
{
	if (this->bCreated == false)
	{
		this->wc.cbSize = sizeof(::WNDCLASSEX);
		this->wc.hIcon = nullptr;
		this->wc.style = CS_HREDRAW | CS_VREDRAW;
		this->wc.lpfnWndProc = helix2d::Window::WndProc;
		this->wc.cbClsExtra = 0;
		this->wc.cbWndExtra = 0;
		this->wc.hInstance = HINST_THISCOMPONENT;
		this->wc.hbrBackground = nullptr;
		this->wc.lpszMenuName = nullptr;
		this->wc.hCursor = ::LoadCursor(HINST_THISCOMPONENT, IDC_ARROW);

		HWND hwParent{ 0 };
		if (this->parent == nullptr)
		{
			WCHAR className[28] = { 0 };
			swprintf_s(className, L"Helix2D %zd", ++ordinal);		//分配类名标识
			this->wc.lpszClassName = className;
			RegisterClassExW(&this->wc);
		}
		else
		{
			//如果有父窗口，则使用父窗口类名标识
			this->wc.lpszClassName = this->parent->wc.lpszClassName;

			hwParent = this->parent->hWnd;

			RegisterClassExW(&this->wc);
		}

		// 计算窗口大小
		DWORD dwStyle = (WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME) | WS_EX_LAYERED;
		RECT wr = { 0, 0, static_cast<LONG>(this->width), static_cast<LONG>(this->height) };
		::AdjustWindowRectEx(&wr, dwStyle, FALSE, NULL);
		// 获取新的宽高
		auto n_width = static_cast<int>(wr.right - wr.left);		//这里的宽高包括标题栏等长度
		auto n_height = static_cast<int>(wr.bottom - wr.top);

		// 获取屏幕分辨率
		int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

		this->hWnd = ::CreateWindowExW(
			NULL,
			this->wc.lpszClassName,
			this->title.c_str(),
			dwStyle,
			static_cast<int>((screenWidth - width) / 2),
			static_cast<int>((screenHeight - height)) / 2,
			n_width, n_height,
			hwParent,
			nullptr,
			HINST_THISCOMPONENT,
			nullptr
		);


		if (this->hWnd == 0)
		{
			//创建窗口失败处理
			UnregisterClassW(this->wc.lpszClassName, HINST_THISCOMPONENT);
		}
		else
		{
			setAlphaWindow();

			ShowWindow(this->hWnd, SW_SHOWNORMAL);
			this->bCreated = true;
		}
	}
}

void helix2d::Window::setAlphaWindow()
{
	if (!::IsWindowsVistaOrGreater())
	{
		return;
	}

	//当前颜色
	DWORD currColor{};

	//颜色是否为透明混合
	BOOL isOpaque{};

	auto hr = ::DwmGetColorizationColor(&currColor, &isOpaque);

	if (!isOpaque || IsWindows8OrGreater())
	{
		HRGN rgn = ::CreateRectRgn(0, 0, -1, -1);
		DWM_BLURBEHIND blurBehind{};
		blurBehind.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
		blurBehind.hRgnBlur = rgn;
		blurBehind.fEnable = true;
		::DwmEnableBlurBehindWindow(hWnd, &blurBehind);
		::DeleteObject(rgn);
	}
	else
	{
		DWM_BLURBEHIND blurBehind{};
		blurBehind.dwFlags = DWM_BB_ENABLE;
		::DwmEnableBlurBehindWindow(hWnd, &blurBehind);
	}
}

void helix2d::Window::Update(float delta)
{
	static MSG msg{};

	static bool fullScreen = false;
	static LONG winStyle{};

	static int originalWidth{};
	static int originalHeight{};

	if (fullScreen != bFullScreen)
	{
		fullScreen = bFullScreen;
		if (fullScreen)
		{
			originalWidth = width;
			originalHeight = height;

			winStyle = GetWindowLongW(hWnd, GWL_STYLE);

			SetWindowLongW(hWnd, GWL_STYLE, WS_POPUP);
			RECT rect;
			GetWindowRect(GetDesktopWindow(), &rect);
			SetWindowPos(
				hWnd, 
				HWND_TOPMOST,
				0, 0,
				static_cast<int>(rect.right), 
				static_cast<int>(rect.bottom),
				SWP_SHOWWINDOW
			);

			width = GetSystemMetrics(SM_CXSCREEN);
			height = GetSystemMetrics(SM_CYSCREEN);
		}
		else
		{
			width = originalWidth;
			height = originalHeight;

			SetWindowLongW(hWnd, GWL_STYLE, winStyle);

			int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
			int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
			SetWindowPos(
				hWnd,
				HWND_TOPMOST,
				static_cast<int>((screenWidth - width) / 2), 
				static_cast<int>((screenHeight - height)) / 2,
				static_cast<int>(width),
				static_cast<int>(height), 
				SWP_SHOWWINDOW
			);
		}
	}

	renderer->createDeviceResources();

	//更新
	if (this->bCreated)
	{
		Tick(delta);
		this->renderer->_render(delta);
	}

	clearInput();
	while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

void helix2d::Window::clearInput()
{
	this->downPressKey.clear();
	this->upPressKey.clear();
	this->downPressMouse.clear();
	this->upPressMouse.clear();
}

helix2d::Window* helix2d::Window::findWindow(HWND hWnd)
{
	auto it = std::find_if(
		allWindows.begin(), allWindows.end(),
		[hWnd](Window* p) {
			if (p->getHWND() == hWnd)
			{
				return true;
			}
			return false;
		}
	);

	if (it == allWindows.end())
	{
		return nullptr;
	}
	return *it;
}

LRESULT CALLBACK helix2d::Window::WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Window* window = Window::findWindow(hWnd);

	if (window == nullptr)
	{
		return DefWindowProcW(hWnd, msg, wparam, lparam);
	}

	switch (msg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		auto key = static_cast<KeyCode>(wparam);
		Input::addKeyCode(window, key);
		Input::addKeyCode(window, Input::getLRKeyCode(key, lparam));
		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		auto key = static_cast<KeyCode>(wparam);
		Input::removeKeyCode(window, key);
		Input::removeKeyCode(window, Input::getLRKeyCode(key, lparam));
		break;
	}
	case WM_LBUTTONDOWN: 
	case WM_MBUTTONDOWN: 
	case WM_RBUTTONDOWN: 
	{
		auto key = MouseCode::Left;
		if (msg == WM_LBUTTONDOWN) { key = MouseCode::Left; }
		else if (msg == WM_MBUTTONDOWN) { key = MouseCode::Mid; }
		else if (msg == WM_RBUTTONDOWN) { key = MouseCode::Right; }

		Input::addMouseCode(window, key);
		break;
	}
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	{
		auto key = MouseCode::Left;
		if (msg == WM_LBUTTONUP) { key = MouseCode::Left; }
		else if (msg == WM_MBUTTONUP) { key = MouseCode::Mid; }
		else if (msg == WM_RBUTTONUP) { key = MouseCode::Right; }

		Input::removeMouseCode(window, key);
		break;
	}
	case WM_ACTIVATE:
	{
		if (LOWORD(wparam) == WA_INACTIVE)
		{
			window->downKey.clear();
			window->downMouse.clear();
		}
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);

		bool bExit{ false };
		while (true)
		{
			if (allWindows.size() == 1)
			{
				bExit = true;
			}
			break;
		}
		if (bExit)
		{
			exit(0);
			break;
		}

		while (true)
		{
			window->bCreated = false;
			window->bWindowDone = true;

			window->hWnd = HWND();
			window->wc = WNDCLASSEXW();
			window->painterList.clear();
			window->clearInput();
			delete window->renderer;

			UnregisterClassW(window->wc.lpszClassName,
				HINST_THISCOMPONENT);

			allWindows.erase(
				std::find(
					allWindows.begin(), allWindows.end(), window)
			);

				
			break;
		}

		break;
	}
	default:
		break;
	}
	return DefWindowProcW(hWnd, msg, wparam, lparam);
}

void helix2d::Window::winControl(Window* window)
{

	auto hr = CoInitializeEx(
		NULL, 
		COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE
	);

	nanoseconds IntervalTime{};
	
	steady_clock::time_point LastTime{};
	steady_clock::time_point NewTime{};
	steady_clock::time_point FixedTime{};

	LastTime = NewTime = FixedTime = steady_clock::now();

	while (true)
	{
		NewTime = steady_clock::now();

		if (window->bWindowDone || bAllWindowDone)
		{
			return;
		}

		if (window->fps > 0)
		{
			IntervalTime = duration_cast<nanoseconds>(seconds(1)) / window->fps;
		}
		else
		{
			IntervalTime = nanoseconds(0);
		}

		if (IntervalTime < (NewTime - FixedTime))
		{
			//窗口创建
			window->create();

			//更新
			window->Update(duration_cast<microseconds>(NewTime - LastTime).count() / 1000.f / 1000.f);

			FixedTime += IntervalTime;

			LastTime = NewTime;
			NewTime = steady_clock::now();
		}
		else
		{
			if (IntervalTime.count())
			{
				// 计算挂起时长
				auto wait = duration_cast<nanoseconds>(IntervalTime - (NewTime - FixedTime));
				if (wait > milliseconds(1))
				{
					// 挂起线程，释放 CPU 占用
					std::this_thread::sleep_for(wait);
				}
			}
		}
	}
}