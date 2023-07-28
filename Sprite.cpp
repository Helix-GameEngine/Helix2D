#include "h2dImage.h"
#include "h2dBase.h"
#include "h2dInbuilt.h"
using namespace helix2d::Inbuilt;

helix2d::Sprite::Sprite()
{
	img = nullptr;
	pD2DImg = nullptr;
	scaleMode = ScaleMode::Linear;
}

helix2d::Sprite::Sprite(const std::wstring& path)
{
	img = nullptr;
	pD2DImg = nullptr;
	scaleMode = ScaleMode::Linear;
	open(path);
}

void helix2d::Sprite::open(const std::wstring& path)
{
	Image::load(path);

	imgPath = path;
}

void helix2d::Sprite::setScaleMode(const ScaleMode& scaleMode)
{
	this->scaleMode = scaleMode;
}

void helix2d::Sprite::Render()
{
	if (img == nullptr)
	{
		img = Image::imgList[imgPath];
		return;
	}

	if (pD2DImg == nullptr)
	{
		auto& d2dimgPair = Image::D2DimgList[img];
		d2dimgPair.second = true;
		if (d2dimgPair.first != nullptr)
		{
			pD2DImg = d2dimgPair.first;
		}
		
		return;
	}

	auto pTarget = window->getRenderer()->pD2D1RenderTarget;
	auto pBrush = window->getRenderer()->pD2D1SolidBrush;

	auto top = getUpperleftPos();

	auto size = pD2DImg->GetSize();

	realWidth = size.width;
	realHeight = size.height;

	auto rect = D2D1::RectF(
		top.x,
		top.y,
		top.x + realWidth,
		top.y + realHeight
	);

	pTarget->DrawBitmap(
		pD2DImg,
		rect,
		color.a,
		static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(scaleMode)
	);
}