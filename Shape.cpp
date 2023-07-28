#include "h2dShape.h"
#include "h2dBase.h"

helix2d::Circle::Circle()
{
	realWidth = 50.0f;
	realHeight = 50.0f;
}

helix2d::Circle::Circle(float radiusX, float radiusY)
{
	realWidth = radiusX * 2;
	realHeight = radiusY * 2;
}

helix2d::Circle::Circle(float radius)
{
	realWidth = radius * 2;
	realHeight = radius * 2;
}

void helix2d::Circle::setRadius(float radiusX, float radiusY)
{
	realWidth = radiusX * 2;
	realHeight = radiusY * 2;
}

void helix2d::Circle::setRadiusX(float radiusX)
{
	realWidth = radiusX * 2;
}

void helix2d::Circle::setRadiusY(float radiusY)
{
	realWidth = radiusY * 2;
}

float helix2d::Circle::getRadiusX() const
{
	return realWidth / 2;
}

float helix2d::Circle::getRadiusY() const
{
	return realHeight / 2;
}

void helix2d::Circle::Render()
{
	auto pTarget = window->getRenderer()->pD2D1RenderTarget;
	auto pBrush = window->getRenderer()->pD2D1SolidBrush;
	
	auto ellipse = D2D1::Ellipse(getWorldPos(), realWidth / 2, realHeight / 2);

	pTarget->FillEllipse(ellipse, pBrush);
}

helix2d::Rect::Rect()
{
	realWidth = 50.0f;
	realHeight = 50.0f;
}

helix2d::Rect::Rect(float width, float height)
{
	this->realWidth = width;
	this->realHeight = height;
}

helix2d::Rect::Rect(float side)
{
	this->realWidth = side;
	this->realHeight = side;
}

void helix2d::Rect::setWidth(float width)
{
	this->realWidth = width;
}

void helix2d::Rect::setHeight(float height)
{
	this->realHeight = height;
}

float helix2d::Rect::getWidth()const
{
	return realWidth;
}

float helix2d::Rect::getHeight()const
{
	return realHeight;
}

void helix2d::Rect::Render()
{
	auto pTarget = window->getRenderer()->pD2D1RenderTarget;
	auto pBrush = window->getRenderer()->pD2D1SolidBrush;

	auto a = getUpperleftPos();

	auto rect = D2D1::Rect(
		getUpperleftPosX(), getUpperleftPosY(),
		getLowerrightPosX(), getLowerrightPosY()
	);

	pBrush->SetColor(color);

	pTarget->FillRectangle(rect, pBrush);
}
