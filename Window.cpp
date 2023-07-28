#include "h2dBase.h"
#include "h2dCamera.h"
using namespace helix2d::Inbuilt;

/******************************************/

size_t helix2d::Window::ordinal{ 0 };

bool helix2d::Window::bAllWindowDone = false;

std::vector<helix2d::Window*> helix2d::Window::allWindows;

std::mutex helix2d::Window::mtx_allwin;

helix2d::Window::Window(
	std::wstring title,
	unsigned int width, unsigned int height,
	Window* parent,
	unsigned int fps
)
{

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

	if(mtx_allwin.try_lock())
	{
		allWindows.push_back(this);
		mtx_allwin.unlock();
	}

	std::thread winThread{ &Window::winControl ,this };
	winThread.detach();
}

helix2d::Window::~Window()
{
	PostMessageW(hWnd, WM_CLOSE, 0, 0);

	bool bExit{ false };
	while (true)
	{
		if (mtx_allwin.try_lock())
		{
			if (allWindows.size() == 1)
			{
				bExit = true;
			}
			mtx_allwin.unlock();
			break;
		}
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
		painter->window = nullptr;	//...
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
	if (std::find(painterList.begin(), painterList.end(), pPainter) 
		!= painterList.end())
	{
		return;
	}

	if (pPainter != nullptr && pPainter->window == nullptr)
	{
		pPainter->setWindow(this);
		painterList.push_back(pPainter);
	}
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
			WCHAR className[21] = { 0 };
			swprintf_s(className, L"%zd", ++ordinal);		//分配类名标识
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
		DWORD dwStyle = WS_OVERLAPPEDWINDOW &~ WS_MAXIMIZEBOX &~ WS_THICKFRAME;
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
			ShowWindow(this->hWnd, SW_SHOWNORMAL);
			this->bCreated = true;
		}
	}
}

void helix2d::Window::update(float delta)
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
		auto it = std::find(
			window->downKey.begin(),
			window->downKey.end(),
			static_cast<KeyCode>(wparam)
		);

		//判断之前是否是松开的
		if (it == window->downKey.end())
		{
			window->downKey.push_back(static_cast<KeyCode>(wparam));
			window->downPressKey.push_back(static_cast<KeyCode>(wparam));
			window->downKeyTime[static_cast<KeyCode>(wparam)] = 
				clock() / 1000.f;
		}
		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		auto it = std::find(
			window->downKey.begin(),
			window->downKey.end(),
			static_cast<KeyCode>(wparam)
		);

		//判断之前是否是按下的
		if (it != window->downKey.end())
		{
			window->downKey.erase(it);
			window->upPressKey.push_back(static_cast<KeyCode>(wparam));

			//不修改时间，以备使用
		}
		else
		{
			window->downKeyTime[static_cast<KeyCode>(wparam)] = -1.f;
		}
		
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

		auto it = std::find(
			window->downMouse.begin(),
			window->downMouse.end(),
			key
		);

		if (it == window->downMouse.end())
		{
			window->downMouse.push_back(key);
			window->downPressMouse.push_back(key);
			window->downMouseTime[key] = clock() / 1000.f;
		}

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

		auto it = std::find(
			window->downMouse.begin(),
			window->downMouse.end(),
			key
		);

		if (it != window->downMouse.end())
		{
			window->downMouse.erase(it);
			window->upPressMouse.push_back(key);
		}
		else
		{
			window->downMouseTime[key] = -1.f;
		}

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
			if (mtx_allwin.try_lock())
			{
				if (allWindows.size() == 1)
				{
					bExit = true;
				}
				mtx_allwin.unlock();
				break;
			}
		}
		if (bExit)
		{
			exit(0);
			break;
		}

		while (true)
		{
			if (mtx_allwin.try_lock())
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

				mtx_allwin.unlock();
				break;
			}
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
	DWORD LastTime = clock();
	DWORD NewTime = LastTime;

	auto hr = CoInitializeEx(
		NULL, 
		COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE
	);

	while (true)
	{
		if (window->bWindowDone || bAllWindowDone)
		{
			return;
		}

		if (NewTime - LastTime >= (DWORD)(1000.0f / window->fps))
		{
			//窗口创建
			window->create();

			//更新
			window->update((clock() - LastTime) / 1000.0f);

			LastTime = NewTime;
		}

		NewTime = clock();
	}
}