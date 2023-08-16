#include "h2dRender.h"
#include "h2dBase.h"
#include "h2dModule.h"
#include "h2dImage.h"

ID2D1Factory* helix2d::Renderer::pD2D1Factory = nullptr;

ID2D1StrokeStyle* helix2d::Renderer::pD2D1MiterStrokeStyle = nullptr;
ID2D1StrokeStyle* helix2d::Renderer::pD2D1BevelStrokeStyle = nullptr;
ID2D1StrokeStyle* helix2d::Renderer::pD2D1RoundStrokeStyle = nullptr;

IWICImagingFactory* helix2d::Renderer::pWICFactory = nullptr;

IDWriteFactory5* helix2d::Renderer::pDWriteFactory = nullptr;

IDWriteTextFormat* helix2d::Renderer::pDWTextFormat = nullptr;

helix2d::Renderer::Renderer(Window* window)
{
	this->window = window;

	pD2D1RenderTarget = nullptr;
	pD2D1SolidBrush = nullptr;

	fDPIScaleX = 0.0f;
	fDPIScaleY = 0.0f;

	bVSyncEnabled = true;
	bDeviceResourceRecreated = false;
}

helix2d::Renderer::~Renderer()
{
	discardDeviceResources();
}

bool helix2d::Renderer::createDeviceIndependentResources()
{
	if (pD2D1Factory != nullptr)
	{
		return true;
	}

	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_MULTI_THREADED,
		&pD2D1Factory
	);

	if (SUCCEEDED(hr))
	{
		hr = pD2D1Factory->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_LINE_JOIN_MITER,
				2.0f,
				D2D1_DASH_STYLE_SOLID,
				0.0f),
			nullptr,
			0,
			&pD2D1MiterStrokeStyle
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = pD2D1Factory->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_LINE_JOIN_BEVEL,
				2.0f,
				D2D1_DASH_STYLE_SOLID,
				0.0f),
			nullptr,
			0,
			&pD2D1BevelStrokeStyle
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = pD2D1Factory->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_LINE_JOIN_ROUND,
				2.0f,
				D2D1_DASH_STYLE_SOLID,
				0.0f),
			nullptr,
			0,
			&pD2D1RoundStrokeStyle
		);
	}

	if (SUCCEEDED(hr))
	{
		// 创建 WIC 绘图工厂，用于统一处理各种格式的图片
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			reinterpret_cast<void**>(&pWICFactory)
		);
	}

	if (SUCCEEDED(hr))
	{
		// 创建 DirectWrite 工厂
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory5),
			reinterpret_cast<IUnknown**>(&pDWriteFactory)
		);
	}

	if (SUCCEEDED(hr))
	{
		// 创建文本格式化对象
		hr = pDWriteFactory->CreateTextFormat(
			L"",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20.0f,
			L"",
			&pDWTextFormat
		);
	}

	if (FAILED(hr))
	{
		discardResources();
	}

	return SUCCEEDED(hr);
}

bool helix2d::Renderer::createDeviceResources()
{
	HRESULT hr = S_OK;

	//需要优化逻辑
	for (auto& imgPair : Image::imgList)
	{
		if (imgPair.second != nullptr &&
			imgPair.second->pConverter != nullptr)
		{
			auto& d2dimgPair = Image::D2DimgList[imgPair.second];
			if (d2dimgPair.second && d2dimgPair.first == nullptr)
			{
				hr = pD2D1RenderTarget->CreateBitmapFromWicBitmap(
					imgPair.second->pConverter,
					nullptr,
					&d2dimgPair.first
				);
			}
		}
	}

	if (pD2D1RenderTarget != nullptr)
	{
		return SUCCEEDED(hr);
	}
	HWND hWnd = window->getHWND();
	
	HDC hdc = ::GetDC(hWnd);
	fDPIScaleX = (float)::GetDeviceCaps(hdc, LOGPIXELSX);
	fDPIScaleY = (float)::GetDeviceCaps(hdc, LOGPIXELSY);

	RECT rc;
	GetClientRect(hWnd, &rc);

	D2D1_SIZE_U size = D2D1::SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top
	);

	if (SUCCEEDED(hr))
	{
		D2D1_RENDER_TARGET_PROPERTIES prop = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);

		hr = pD2D1Factory->CreateHwndRenderTarget(
			prop,
			D2D1::HwndRenderTargetProperties(
				hWnd,
				size,
				bVSyncEnabled ? D2D1_PRESENT_OPTIONS_NONE : D2D1_PRESENT_OPTIONS_IMMEDIATELY
			),
			&pD2D1RenderTarget
		);
	}

	if (SUCCEEDED(hr))
	{
		// 创建画刷
		hr = pD2D1RenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&pD2D1SolidBrush
		);
	}

	return SUCCEEDED(hr);
}

void helix2d::Renderer::discardDeviceResources()
{
	SafeRelease(pD2D1RenderTarget);
	SafeRelease(pD2D1SolidBrush);
	bDeviceResourceRecreated = true;
}

void helix2d::Renderer::discardResources()
{
	SafeRelease(pD2D1Factory);
	SafeRelease(pWICFactory);
	SafeRelease(pDWriteFactory);
	SafeRelease(pD2D1MiterStrokeStyle);
	SafeRelease(pD2D1BevelStrokeStyle);
	SafeRelease(pD2D1RoundStrokeStyle);
	SafeRelease(pDWTextFormat);
}

void helix2d::Renderer::_render(float delta)
{
	if (pD2D1RenderTarget == nullptr)
	{
		return;
	}

	HRESULT hr = S_OK;

	//绘制背景
	pD2D1RenderTarget->BeginDraw();
	pD2D1RenderTarget->Clear(window->backgroundColor);

	Painter::updateTick(window, delta);

	Module::updateModule(window, delta);

	Painter::updateEnd(window, delta);

	Painter::updateRender(window);

	hr = pD2D1RenderTarget->EndDraw();
}

helix2d::Color::Color()
{
	setColor(White);
}

helix2d::Color::Color(float r, float g, float b, float a)
{
	setColor(r, g, b, a);
}

helix2d::Color::Color(long rgb, float a)
{
	setColor(rgb, a);
}

void helix2d::Color::setColor(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

void helix2d::Color::setColor(long rgb, float a)
{
	auto&& c = D2D1::ColorF(rgb, a);

	r = c.r;
	g = c.g;
	b = c.b;
	this->a = a;
}

helix2d::Color::operator D2D1_COLOR_F()
{
	return D2D1::ColorF(r, g, b, a);
}
