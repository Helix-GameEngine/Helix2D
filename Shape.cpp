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

void helix2d::Rect::setRealWidth(float width)
{
	this->realWidth = width;
}

void helix2d::Rect::setRealHeight(float height)
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

helix2d::Line::Line()
{
	setLineWidth(2.0f);
}

helix2d::Line::Line(Vector2 ep1, Vector2 ep2, float width)
{
	setEndpoint(ep1, ep2);
	setLineWidth(width);
}

void helix2d::Line::setEndpoint(Vector2 ep1, Vector2 ep2)
{
	endpoint1 = ep1;
	endpoint2 = ep2;

	realWidth = max(abs(endpoint1.x), abs(endpoint2.x));
	realHeight = max(abs(endpoint1.y), abs(endpoint2.y));
}

void helix2d::Line::setLineWidth(float width)
{
	lineWidth = width;
}

void helix2d::Line::getEndpoint(Vector2& ep1, Vector2& ep2) const
{
	ep1 = endpoint1;
	ep2 = endpoint2;
}

float helix2d::Line::getLineWidth() const
{
	return lineWidth;
}

float helix2d::Line::getRealBoundingWidth() const
{
	return realWidth + lineWidth * 0.75f;
}

float helix2d::Line::getRealBoundingHeight() const
{
	return realHeight + lineWidth / 2.0f;
}

float helix2d::Line::getBoundingWidth() const
{
	auto x = width / realWidth;
	return width + lineWidth * x * 0.75f;
}

float helix2d::Line::getBoundingHeight() const
{
	auto y = height / realHeight;
	return height + lineWidth * y / 2.0f;
}

void helix2d::Line::Render()
{
	auto* pTarget = window->getRenderer()->pD2D1RenderTarget;
	auto* pBrush = window->getRenderer()->pD2D1SolidBrush;
	auto* pStyle = window->getRenderer()->pD2D1RoundStrokeStyle;

	auto upperleft = getUpperleftPos();

	auto p1 = upperleft + endpoint1;
	auto p2 = upperleft + endpoint2;

	pTarget->DrawLine(p1, p2, pBrush, lineWidth, pStyle);
}
