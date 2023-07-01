#include "h2dShape.h"
#include "h2dBase.h"

helix2d::Circle::Circle()
{
	width = 50.0f;
	height = 50.0f;
}

helix2d::Circle::Circle(float radiusX, float radiusY)
{
	width = radiusX * 2;
	height = radiusY * 2;
}

helix2d::Circle::Circle(float radius)
{
	width = radius * 2;
	height = radius * 2;
}

void helix2d::Circle::Render()
{
	auto pTarget = window->getRenderer()->pD2D1RenderTarget;
	auto pBrush = window->getRenderer()->pD2D1SolidBrush;
	
	auto ellipse = D2D1::Ellipse(getTopLeftPos(), width / 2, height / 2);

	pBrush->SetColor(color);

	pTarget->FillEllipse(ellipse, pBrush);
}

helix2d::Rect::Rect()
{
	width = 50.0f;
	height = 50.0f;
}

helix2d::Rect::Rect(float width, float height)
{
	this->width = width;
	this->height = height;
}

helix2d::Rect::Rect(float side)
{
	this->width = side;
	this->height = side;
}

void helix2d::Rect::setWidth(float width)
{
	this->width = width;
}

void helix2d::Rect::setHeight(float height)
{
	this->height = height;
}

void helix2d::Rect::Render()
{
	auto pTarget = window->getRenderer()->pD2D1RenderTarget;
	auto pBrush = window->getRenderer()->pD2D1SolidBrush;

	auto a = getTopLeftPos();

	auto rect = D2D1::Rect(
		getTopLeftPosX(), getTopLeftPosY(),
		getBottomRightPosX(), getBottomRightPosY()
	);

	pBrush->SetColor(color);

	pTarget->FillRectangle(rect, pBrush);
}
