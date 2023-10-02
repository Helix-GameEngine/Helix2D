#include "h2dModule.h"
#include "h2dBase.h"
#include "h2dRender.h"

std::vector<helix2d::Module*> helix2d::Module::allMod;

helix2d::Module::Module()
{
	parent = nullptr;
	modType = Type::Modifier;

	allMod.push_back(this);
}

helix2d::Module::~Module()
{
	auto it = std::find(allMod.begin(), allMod.end(), this);

	if (it != allMod.end())
	{
		allMod.erase(it);
	}
}

helix2d::Painter* helix2d::Module::getParent() const
{
	return parent;
}

helix2d::Module::Type helix2d::Module::getModuleType()const
{
	return modType;
}

void helix2d::Module::updateModule(Window* window, float delta)
{
	updateTypeModule(window, delta, Type::Modifier);
	updateTypeModule(window, delta, Type::Detector);
}

std::vector<helix2d::Module*> helix2d::Module::getAllModule()
{
	return allMod;
}

void helix2d::Module::updateTypeModule(Window* window, float delta, Type type)
{
	for (size_t i = 0; i < Module::allMod.size(); i++)
	{
		auto* mod = allMod[i];
		if (mod->parent == nullptr)
		{
			continue;
		}

		if (mod->parent->getWindow() != window)
		{
			continue;
		}

		if (!mod->parent->isEnableUpdate())
		{
			continue;
		}

		if (mod->modType == type)
		{
			mod->Update(delta);
		}
	}
}

helix2d::Gravity::Gravity(float g)
{
	gravity = g;
}

void helix2d::Gravity::setGravity(float g)
{
	gravity = g;
}

float helix2d::Gravity::getGravity() const
{
	return gravity;
}

void helix2d::Gravity::Update(float delta)
{
	parent->addVelocity(0.0f, gravity * delta * 50.0f * 9.8f);
}


ID2D1Geometry* helix2d::Mask::currGeometry = nullptr;
std::map<helix2d::Painter*, ID2D1Geometry*> helix2d::Mask::lastGeometries;
bool helix2d::Mask::isPushLayer = false;

helix2d::Mask::Mask()
{
	pLayer = nullptr;
	pGeoSink = nullptr;
	pPathGeo = nullptr;

	isInverse = false;

	width = 50.0f;
	height = 50.0f;
}

helix2d::Mask::Mask(float width, float height) :Mask()
{
	this->width = width;
	this->height = height;
}

helix2d::Mask::~Mask()
{
	SafeRelease(pLayer);
	SafeRelease(pGeoSink);
	SafeRelease(pPathGeo);
}

void helix2d::Mask::setRealWidth(float width)
{
	this->width = width;
}

void helix2d::Mask::setRealHeight(float height)
{
	this->height = height;
}

void helix2d::Mask::setInverse(bool inverse)
{
	isInverse = inverse;
}

void helix2d::Mask::setPos(Vector2 pos)
{
	this->pos = pos;
}

void helix2d::Mask::setPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

helix2d::Vector2 helix2d::Mask::getPos()
{
	return pos;
}

void helix2d::Mask::BeforeRender()
{
	HRESULT hr = S_OK;

	auto window = parent->getWindow();
	Vector2 worldPos;
	worldPos.x = parent->getUpperleftPos().x + pos.x;
	worldPos.y = parent->getUpperleftPos().y + pos.y;

	auto* pFactory = window->getRenderer()->pD2D1Factory;
	auto* pTarget = window->getRenderer()->pD2D1RenderTarget;

	//检查parent有无保存上一currGeometry的值
	if (lastGeometries.count(parent) == 0)		//无，则保存
	{
		lastGeometries[parent] = currGeometry;
	}											//有，则不做任何工作

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
				D2D1::RectF(worldPos.x, worldPos.y, worldPos.x + width, worldPos.y + height),
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
		parent->getWindow()->getRenderer()->pD2D1RenderTarget->PopLayer();
		isPushLayer = false;
	}

	//检查是否已经设置及清除过
	if (lastGeometries.count(parent))		//否，设置并清除map元素
	{
		currGeometry = lastGeometries[parent];
		lastGeometries.erase(parent);
	}										//是，则不做任何工作

	SafeRelease(pPathGeo);
}
