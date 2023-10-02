#include "h2dRender.h"
#include "h2dBase.h"
#include "h2dModule.h"
#include "h2dCamera.h"
#include <cmath>
#include <algorithm>

helix2d::Vector2 helix2d::Painter::defaultAnchor{ 0.5f,0.5f };

helix2d::Painter::Painter()
{
	anchor = defaultAnchor;
	angle = 0.0f;
	scale = Scale(1.0f, 1.0f);

	realWidth = 0.0f;
	realHeight = 0.0f;

	width = 0.0f;
	height = 0.0f;

	fFlipX = 1.0f;
	fFlipY = 1.0f;

	realPos = pos;

	order = 1;

	mass = 1.0f;

	bEnableTick = true;
	bEnableRender = true;
	bEnableUpdate = true;

	bSortModule = false;
	bSortOrder = false;

	window = nullptr;
}

void helix2d::Painter::addPainter(Painter* pPainter)
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

	pPainter->setWindow(window);
	pPainter->parent = this;

	painterList.push_back(pPainter);

	updateProperty();
}

void helix2d::Painter::addPainter(Painter* pPainter, std::wstring name)
{
	pPainter->setName(name);
	addPainter(pPainter);
}

void helix2d::Painter::addModule(Module* pMod)
{
	if (pMod == nullptr)
	{
		return;
	}

	if (pMod->parent)
	{
		return;
	}

	pMod->parent = this;
	modList.push_back(pMod);
	bSortModule = true;
}

bool helix2d::Painter::removePainter(Painter* pPainter)
{
	if (SafeRemove(painterList, pPainter))
	{
		pPainter->parent = nullptr;
		pPainter->setWindow(nullptr);
		return true;
	}
	return false;
}

bool helix2d::Painter::removeModule(Module* pMod)
{
	if (SafeRemove(modList, pMod))
	{
		pMod->parent = nullptr;
		return true;
	}
	return false;
}

helix2d::Painter* helix2d::Painter::findPainter(std::wstring name)
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

void helix2d::Painter::addVelocity(Vector2 a)
{
	velocity += a;
}

void helix2d::Painter::addVelocity(float ax, float ay)
{
	velocity.x += ax;
	velocity.y += ay;
}

void helix2d::Painter::addVelocityX(float a)
{
	velocity.x += a;
}

void helix2d::Painter::addVelocityY(float a)
{
	velocity.y += a;
}

void helix2d::Painter::addVelocityAhead(float size)
{
	addVelocityAhead(size, angle);
}

void helix2d::Painter::addVelocityAhead(float size, float angle)
{
	Vector2 v{
		cosf(Math::getRadian(angle - 90)),
		sinf(Math::getRadian(angle - 90))
	};
	v *= size;
	addVelocity(v);
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
	updateProperty();
}

void helix2d::Painter::setScale(float scaleX, float scaleY)
{
	scale = Scale(scaleX, scaleY);
	updateProperty();
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

void helix2d::Painter::setVelocity(Vector2 v)
{
	velocity = v;
}

void helix2d::Painter::setVelocity(float vx, float vy)
{
	velocity.x = vx;
	velocity.y = vy;
}

void helix2d::Painter::setVelocityX(float vx)
{
	velocity.x = vx;
}

void helix2d::Painter::setVelocityY(float vy)
{
	velocity.y = vy;
}

void helix2d::Painter::setOrder(size_t nOrder)
{
	order = nOrder;
	bSortOrder = true;
}

void helix2d::Painter::setName(std::wstring name)
{
	this->name = name;
}

void helix2d::Painter::setMass(float m)
{
	mass = m;
}

void helix2d::Painter::enableTick(bool b)
{
	bEnableTick = b;
}

void helix2d::Painter::enableRender(bool b)
{
	bEnableRender = b;
}

void helix2d::Painter::enableUpdate(bool b)
{
	bEnableUpdate = b;
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

void helix2d::Painter::moveAhead(float size)
{
	moveAhead(size, angle);
}

void helix2d::Painter::moveAhead(float size, float angle)
{
	Vector2 v{
		cosf(Math::getRadian(angle - 90)),
		sinf(Math::getRadian(angle - 90))
	};
	v *= size;
	movePos(v);
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

std::vector<helix2d::Module*> helix2d::Painter::getAllModule() const
{
	return modList;
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

float helix2d::Painter::getRealWidth() const
{
	return realWidth;
}

float helix2d::Painter::getRealHeight() const
{
	return realHeight;
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

helix2d::Vector2 helix2d::Painter::getDisplayUpperleftPos() const
{
	return Vector2(
		worldPos.x - width * anchor.x,
		worldPos.y - height * anchor.y
	);
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

	auto pos1 = getDisplayUpperleftPos();
	auto pos2 = other->getDisplayUpperleftPos();

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

bool helix2d::Painter::isEnableRender() const
{
	return bEnableRender;
}

bool helix2d::Painter::isEnableUpdate() const
{
	return bEnableUpdate;
}

void helix2d::Painter::setDefaultAnchor(Vector2 pos)
{
	defaultAnchor = pos;
}

void helix2d::Painter::setDefaultAnchor(float x, float y)
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

void helix2d::Painter::updateTick(Window* window, float delta)
{
	if (window)
	{
		for (size_t i = 0; i < window->getAllPainter().size(); i++)
		{
			window->getAllPainter()[i]->_update(delta);
		}
	}
}

void helix2d::Painter::updateRender(Window* window)
{
	if (window)
	{
		for (size_t i = 0; i < window->getAllPainter().size(); i++)
		{
			window->getAllPainter()[i]->paint();
		}
	}
}

void helix2d::Painter::updateEnd(Window* window, float delta)
{
	if (window)
	{
		for (auto& painter : window->getAllPainter())
		{
			painter->updateEnd(delta);
		}
	}
}

void helix2d::Painter::paint()
{
	_render();
}

void helix2d::Painter::paint(Vector2 pos, float angle)
{
	_render(pos, angle, window);
}

void helix2d::Painter::paint(Window* window)
{
	_render(pos, angle, window);
}

void helix2d::Painter::paint(Vector2 pos, float angle, Window* window)
{
	_render(pos, angle, window);
}

helix2d::Vector2 helix2d::Painter::getUpperleftPos() const
{
	return Vector2(
		worldPos.x - realWidth * anchor.x,
		worldPos.y - realHeight * anchor.y
	);
}

float helix2d::Painter::getUpperleftPosX() const
{
	return getUpperleftPos().x;
}

float helix2d::Painter::getUpperleftPosY() const
{
	return getUpperleftPos().y;
}

helix2d::Vector2 helix2d::Painter::getLowerrightPos() const
{
	return Vector2(getUpperleftPosX() + realWidth, getUpperleftPosY() + realHeight);
}

float helix2d::Painter::getLowerrightPosX() const
{
	return getLowerrightPos().x;
}

float helix2d::Painter::getLowerrightPosY() const
{
	return getLowerrightPos().y;
}

helix2d::Vector2 helix2d::Painter::getUpperrightPos() const
{
	return Vector2(getUpperleftPosX() + realWidth,getUpperleftPosY());
}

float helix2d::Painter::getUpperrightPosX() const
{
	return getUpperrightPos().x;
}

float helix2d::Painter::getUpperrightPosY() const
{
	return getUpperrightPos().y;
}

helix2d::Vector2 helix2d::Painter::getLowerleftPos() const
{
	return Vector2(getUpperleftPosX(),getUpperleftPosY() + realHeight);
}

float helix2d::Painter::getLowerleftPosX() const
{
	return getLowerleftPos().x;
}

float helix2d::Painter::getLowerleftPosY() const
{
	return getLowerleftPos().y;
}

helix2d::Vector2 helix2d::Painter::getVelocity() const
{
	return velocity;
}

float helix2d::Painter::getVelocityX() const
{
	return velocity.x;
}

float helix2d::Painter::getVelocityY() const
{
	return velocity.y;
}

float helix2d::Painter::getMass() const
{
	return mass;
}

size_t helix2d::Painter::getOrder() const
{
	return order;
}

std::wstring helix2d::Painter::getName() const
{
	return name;
}

void helix2d::Painter::_render()
{
	if (bEnableRender)
	{
		if (window == nullptr)
		{
			return;
		}

		auto& pTarget = window->getRenderer()->pD2D1RenderTarget;
		auto& pBrush = window->getRenderer()->pD2D1SolidBrush;
		pBrush->SetColor(color);

		Matrix3x2 matrix = D2D1::Matrix3x2F::Identity();
		Matrix3x2 original = D2D1::Matrix3x2F::Identity();

		pTarget->GetTransform(&original);
		matrix = matrix * original;

		//摄像机变换
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

		for (size_t i = 0; i < modList.size(); i++)
		{
			modList[i]->BeforeRender();
		}

		Render();

		for (auto painter : painterList)
		{
			painter->paint();
		}

		AfterChildRender();

		for (size_t i = 0; i < modList.size(); i++)
		{
			modList[i]->AfterChildRender();
		}

		pTarget->SetTransform(original);
	}
}

void helix2d::Painter::_render(Vector2 pos, float angle, Window* window)
{
	auto m_pos = this->realPos;
	auto m_angle = this->angle;
	auto m_window = this->window;

	this->realPos = pos;
	this->angle = angle;
	this->window = window;

	updateProperty();

	_render();

	this->realPos = m_pos;
	this->angle = m_angle;
	this->window = m_window;

	updateProperty();
}

void helix2d::Painter::_update(float delta)
{
	if (bEnableUpdate)
	{
		sortOrder();
		sortModule();

		if (bEnableTick)
		{
			Tick(delta);
		}

		for (auto painter : painterList)
		{
			painter->_update(delta);
		}
	}
}

void helix2d::Painter::_updateModule(float delta)	//备用
{
	for (auto painter : painterList)
	{
		painter->_updateModule(delta);
	}

	for (auto pMod : modList)
	{
		pMod->Update(delta);
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
		realScale = scale;
	}
	else
	{
		worldPos = parent->worldPos + pos;
		rotationMatrix = D2D1::Matrix3x2F::Rotation(angle, worldPos) * parent->rotationMatrix;
		realScale.x = scale.x * parent->realScale.x;
		realScale.y = scale.y * parent->realScale.y;
	}

	realPos = rotationMatrix.TransformPoint(worldPos);

	width = abs(realScale.x) * realWidth;
	height = abs(realScale.y) * realHeight;

	UpdateProperty();

	for (auto& painter : painterList)
	{
		painter->updateProperty();
	}
}

void helix2d::Painter::updateEnd(float delta)
{
	if (bEnableUpdate)
	{
		// 模拟物体受到阻力作用，减慢速度
		float drag = 0.9f;
		velocity *= 1.0f / (1.0f + delta * drag);
		movePos(velocity * delta);

		for (auto& p : painterList)
		{
			p->updateEnd(delta);
		}
	}
}

void helix2d::Painter::sortModule()
{
	if (modList.empty())
	{
		return;
	}

	if (bSortModule)
	{
		std::sort(
			modList.begin(), modList.end(),
			[](const Module* mod1, const Module* mod2)
			{
				return mod1->getModuleType() < mod2->getModuleType();
			}
		);
		bSortModule = false;
	}
}

void helix2d::Painter::sortOrder()
{
	if (painterList.empty())
	{
		return;
	}

	if (bSortOrder)
	{
		std::sort(
			painterList.begin(), painterList.end(),
			[](const Painter* painter1, const Painter* painter2)
			{
				return painter1->order < painter2->order;
			}
		);
		bSortOrder = false;
	}
}
