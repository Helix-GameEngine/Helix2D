#include "h2dMask.h"
#include "h2dBase.h"

ID2D1Geometry* helix2d::Mask::currGeometry = nullptr;
bool helix2d::Mask::isPushLayer = false;

helix2d::Mask::Mask()
{
	pLayer = nullptr;
	pGeoSink = nullptr;
	pPathGeo = nullptr;

	lastGeometry = nullptr;

	isInverse = false;

	realWidth = 50.0f;
	realHeight = 50.0f;
}

helix2d::Mask::~Mask()
{
	SafeRelease(pLayer);
	SafeRelease(pGeoSink);
	SafeRelease(pPathGeo);
}

void helix2d::Mask::setWidth(float width)
{
	realWidth = width;
}

void helix2d::Mask::setHeight(float height)
{
	realHeight = height;
}

void helix2d::Mask::setInverse(bool inverse)
{
	isInverse = inverse;
}

void helix2d::Mask::Render()
{
	HRESULT hr = S_OK;

	auto* pFactory = window->getRenderer()->pD2D1Factory;
	auto* pTarget = window->getRenderer()->pD2D1RenderTarget;

	lastGeometry = currGeometry;

	if (pPathGeo == nullptr)
	{
		hr = pFactory->CreatePathGeometry(&pPathGeo);
	}

	if (SUCCEEDED(hr))
	{
		hr = pPathGeo->Open(&pGeoSink);
	}

	if (SUCCEEDED(hr))
	{
		ID2D1RectangleGeometry* screenGeometry{};
		ID2D1RectangleGeometry* rectGeometry{};

		if (SUCCEEDED(hr))
		{
			hr = pFactory->CreateRectangleGeometry(
				D2D1::RectF(0.0f, 0.0f, static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight())),
				&screenGeometry
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFactory->CreateRectangleGeometry(
				D2D1::RectF(getUpperleftPos().x, getUpperleftPos().y, getLowerrightPos().x, getLowerrightPos().y),
				&rectGeometry
			);
		}

		if (currGeometry == nullptr)
		{
			currGeometry = screenGeometry;
		}

		pGeoSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		if (SUCCEEDED(hr))
		{
			if (isInverse)
			{
				hr = currGeometry->CombineWithGeometry(rectGeometry, D2D1_COMBINE_MODE_INTERSECT, nullptr, pGeoSink);
			}
			else
			{
				hr = currGeometry->CombineWithGeometry(rectGeometry, D2D1_COMBINE_MODE_EXCLUDE, nullptr, pGeoSink);
			}
		}

		if (SUCCEEDED(hr))
		{
			hr = pGeoSink->Close();
			SafeRelease(screenGeometry);
			SafeRelease(rectGeometry);
			currGeometry = nullptr;
		}

		SafeRelease(pGeoSink);
	}

	if (SUCCEEDED(hr))
	{
		currGeometry = pPathGeo;
	}

	if (pLayer == nullptr && SUCCEEDED(hr))
	{
		hr = pTarget->CreateLayer(&pLayer);
	}

	if (SUCCEEDED(hr))
	{
		if (isPushLayer)
		{
			pTarget->PopLayer();
		}

		pTarget->PushLayer(
			D2D1::LayerParameters(D2D1::InfiniteRect(), pPathGeo),
			pLayer
		);
		isPushLayer = true;
	}
}

void helix2d::Mask::AfterChildRender()
{
	if (isPushLayer)
	{
		window->getRenderer()->pD2D1RenderTarget->PopLayer();
		isPushLayer = false;
	}
	currGeometry = lastGeometry;
	SafeRelease(pPathGeo);
}
