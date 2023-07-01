#include "h2dRender.h"
#include "h2dBase.h"
#include "h2dModule.h"
#include "h2dCamera.h"
#include <cmath>

helix2d::Vector2 helix2d::Painter::defaultAnchor{ 0.5f,0.5f };

helix2d::Painter::Painter()
{
	anchor = defaultAnchor;
	angle = 0.0f;

	scale = Scale(1.0f, 1.0f);

	fFlipX = 1.0f;
	fFlipY = 1.0f;

	realPos = pos;

	bEnableTick = true;

	window = nullptr;
}

void helix2d::Painter::addPainter(Painter* pPainter)
{
	if (pPainter == nullptr)
	{
		return;
	}

	if (pPainter->parent || pPainter->window)
	{
		return;
	}

	pPainter->setWindow(window);
	pPainter->parent = this;

	painterList.push_back(pPainter);

	updateProperty();
}

void helix2d::Painter::addModule(Module* pMod)
{
	if (std::find(modList.begin(), modList.end(), pMod) != modList.end())
	{
		return;
	}
	pMod->parent = this;
	modList.push_back(pMod);
}

void helix2d::Painter::rotate(float angle)
{
	this->angle += angle;
	this->angle = fmodf(this->angle, 360.0f);
	updateProperty();
}

void helix2d::Painter::flipX()
{
	fFlipX = -fFlipX;
}

void helix2d::Painter::flipY()
{
	fFlipY = -fFlipY;
}

void helix2d::Painter::setPos(float x, float y)
{
	pos.x = x;
	pos.y = y;

	updateProperty();
}

void helix2d::Painter::setPos(helix2d::Vector2 pos)
{
	setPos(pos.x, pos.y);
}

void helix2d::Painter::setPosX(float x)
{
	setPos(x, pos.y);
}

void helix2d::Painter::setPosY(float y)
{
	setPos(pos.x, y);
}

void helix2d::Painter::setScale(Scale scale)
{
	this->scale = scale;
}

void helix2d::Painter::setScale(float scaleX, float scaleY)
{
	scale = Scale(scaleX, scaleY);
}

void helix2d::Painter::setAnchor(Vector2 anchor)
{
	this->anchor = anchor;
}

void helix2d::Painter::setAnchor(float x, float y)
{
	anchor.x = x;
	anchor.y = y;
	
	updateProperty();
}

void helix2d::Painter::setAngle(float angle)
{
	this->angle = angle;
	this->angle = fmodf(this->angle, 360.0f);
	updateProperty();
}

void helix2d::Painter::setColor(Color c)
{
	color = c;
}

void helix2d::Painter::setFlipX(bool flipX)
{
	if (flipX)
	{
		fFlipX = -1.0f;
	}
	else
	{
		fFlipX = 1.0f;
	}
}

void helix2d::Painter::setFlipY(bool flipY)
{
	if (flipY)
	{
		fFlipY = -1.0f;
	}
	else
	{
		fFlipY = 1.0f;
	}
}

void helix2d::Painter::enableTick(bool b)
{
	bEnableTick = b;
}

void helix2d::Painter::movePos(float x, float y)
{
	movePos({ x,y });
}

void helix2d::Painter::movePos(Vector2 pos)
{
	this->pos += pos;
	updateProperty();
}

void helix2d::Painter::movePosX(float x)
{
	movePos(x, 0.0f);
}

void helix2d::Painter::movePosY(float y)
{
	movePos(0.0f, y);
}

helix2d::Window* helix2d::Painter::getWindow() const
{
	return window;
}

helix2d::Painter* helix2d::Painter::getParent() const
{
	return parent;
}

std::vector<helix2d::Painter*> helix2d::Painter::getAllPainter() const
{
	return painterList;
}

helix2d::Vector2 helix2d::Painter::getPos() const
{
	return pos;
}

float helix2d::Painter::getPosX() const
{
	return pos.x;
}

float helix2d::Painter::getPosY() const
{
	return pos.y;
}

float helix2d::Painter::getWidth() const
{
	return width;
}

float helix2d::Painter::getHeight() const
{
	return height;
}

helix2d::Scale helix2d::Painter::getScale() const
{
	return scale;
}

float helix2d::Painter::getScaleX() const
{
	return scale.x;
}

float helix2d::Painter::getScaleY() const
{
	return scale.y;
}

helix2d::Vector2 helix2d::Painter::getAnchor() const
{
	return anchor;
}

float helix2d::Painter::getAngle() const
{
	return angle;
}

helix2d::Vector2 helix2d::Painter::getWorldPos() const
{
	return worldPos;
}

helix2d::Vector2 helix2d::Painter::getRealPos() const
{
	return realPos;
}

bool helix2d::Painter::isOverlap(const Painter* other) const
{
	if (other == nullptr)
	{
		return false;
	}

	if (window != other->window)
	{
		return false;
	}

	auto pos1 = this->getTopLeftPos();
	auto pos2 = other->getTopLeftPos();
	
	if (
		(pos1.x >= pos2.x && pos1.x < pos2.x + other->width) ||
		(pos1.x + width >= pos2.x && pos1.x + width < pos2.x + other->width)
	)
	{
		if (
			(pos1.y >= pos2.y && pos1.y < pos2.y + other->height) ||
			(pos1.y + height >= pos2.y && pos1.y + height < pos2.y + other->height)
		)
		{
			return true;
		}
	}

	return false;
}

bool helix2d::Painter::isEnableTick() const
{
	return bEnableTick;
}

void helix2d::Painter::setDefaultAnchorPos(Vector2 pos)
{
	defaultAnchor = pos;
}

void helix2d::Painter::setDefaultAnchorPos(float x, float y)
{
	defaultAnchor.x = x;
	defaultAnchor.y = y;
}

void helix2d::Painter::updateProperty(Window* window)
{
	if (window)
	{
		for (auto& painter : window->getAllPainter())
		{
			painter->updateProperty();
		}
	}
}

void helix2d::Painter::Paint()
{
	_render();
}

void helix2d::Painter::Paint(Vector2 pos, float angle)
{
	_render(pos, angle, window);
}

void helix2d::Painter::Paint(Window* window)
{
	_render(pos, angle, window);
}

void helix2d::Painter::Paint(Vector2 pos, float angle, Window* window)
{
	_render(pos, angle, window);
}

helix2d::Vector2 helix2d::Painter::getTopLeftPos() const
{
	/*return Vector2(
		displayPos.x - width * anchor.x,
		displayPos.y - height * anchor.y
	);*/

	return Vector2(
		worldPos.x - width * anchor.x,
		worldPos.y - height * anchor.y
	);
}

float helix2d::Painter::getTopLeftPosX() const
{
	return getTopLeftPos().x;
}

float helix2d::Painter::getTopLeftPosY() const
{
	return getTopLeftPos().y;
}

helix2d::Vector2 helix2d::Painter::getBottomRightPos() const
{
	return Vector2(getTopLeftPosX() + width, getTopLeftPosY() + height);
}

float helix2d::Painter::getBottomRightPosX() const
{
	return getBottomRightPos().x;
}

float helix2d::Painter::getBottomRightPosY() const
{
	return getBottomRightPos().y;
}

helix2d::Vector2 helix2d::Painter::getTopRightPos() const
{
	return Vector2(getTopLeftPosX() + width,getTopLeftPosY());
}

float helix2d::Painter::getTopRightPosX() const
{
	return getTopRightPos().x;
}

float helix2d::Painter::getTopRightPosY() const
{
	return getTopRightPos().y;
}

helix2d::Vector2 helix2d::Painter::getBottomLeftPos() const
{
	return Vector2(getTopLeftPosX(),getTopLeftPosY() + height);
}

float helix2d::Painter::getBottomLeftPosX() const
{
	return getBottomLeftPos().x;
}

float helix2d::Painter::getBottomLeftPosY() const
{
	return getBottomLeftPos().y;
}

void helix2d::Painter::_render()
{
	if (window == nullptr)
	{
		return;
	}

	if (parent == nullptr)
	{
		updateProperty();
	}

	auto& pTarget = window->getRenderer()->pD2D1RenderTarget;
	auto& pBrush = window->getRenderer()->pD2D1SolidBrush;
	pBrush->SetColor(color);

	Matrix3x2 matrix = D2D1::Matrix3x2F::Identity();
	Matrix3x2 original = D2D1::Matrix3x2F::Identity();

	pTarget->GetTransform(&original);
	matrix = matrix * original;

	//ÉãÏñ»ú±ä»»
	if (window->getCamera() != nullptr)
	{
		Painter* camera = window->getCamera();

		auto x = window->getWidth() / 2.0f - camera->worldPos.x;
		auto y = window->getHeight() / 2.0f - camera->worldPos.y;

		matrix = matrix * D2D1::Matrix3x2F::Rotation(-camera->angle, camera->worldPos);

		matrix = matrix * D2D1::Matrix3x2F::Translation(
			D2D1::SizeF(x, y)
		);
	}

	pTarget->SetTransform(
		D2D1::Matrix3x2F::Rotation(angle, worldPos) *
		D2D1::Matrix3x2F::Scale(scale.x, scale.y, worldPos) *
		D2D1::Matrix3x2F::Scale(fFlipY, fFlipX, worldPos) *
		matrix
	);

	Render();

	for (auto painter : painterList)
	{
		painter->Paint();
	}

	pTarget->SetTransform(original);
}

void helix2d::Painter::_render(Vector2 pos, float angle, Window* window)
{
	auto m_pos = this->realPos;
	auto m_angle = this->angle;
	auto m_window = this->window;

	this->realPos = pos;
	this->angle = angle;
	this->window = window;

	_render();

	this->realPos = m_pos;
	this->angle = m_angle;
	this->window = m_window;

	updateProperty();
}

void helix2d::Painter::_update(float delta)
{
	if (bEnableTick)
	{
		Tick(delta);
	}

	for (auto painter : painterList)
	{
		painter->_update(delta);
	}
}

void helix2d::Painter::_updateModule(float delta)
{
	for (auto painter : painterList)
	{
		painter->_updateModule(delta);
	}

	for (auto pMod : modList)
	{
		pMod->update(delta);
	}
}

void helix2d::Painter::setWindow(Window* window)
{
	this->window = window;
	for (auto& painter : painterList)
	{
		painter->setWindow(window);
	}
}

void helix2d::Painter::updateProperty()
{
	
	if (parent == nullptr)
	{
		worldPos = pos;
		rotationMatrix = D2D1::Matrix3x2F::Rotation(angle, worldPos);
	}
	else
	{
		worldPos = parent->worldPos + pos;
		rotationMatrix = D2D1::Matrix3x2F::Rotation(angle, worldPos) * parent->rotationMatrix;
	}

	realPos = rotationMatrix.TransformPoint(worldPos);

	for (auto& painter : painterList)
	{
		painter->updateProperty();
	}
}
