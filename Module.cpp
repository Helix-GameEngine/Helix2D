#include "h2dModule.h"
#include "h2dBase.h"
#include "h2dRender.h"

std::vector<helix2d::Module*> helix2d::Module::allMod;

helix2d::Module::Module()
{
	parent = nullptr;

	allMod.push_back(this);
}

helix2d::Module::~Module()
{
	allMod.erase(std::find(allMod.begin(), allMod.end(), this));
}

helix2d::Painter* helix2d::Module::getParent() const
{
	return parent;
}

std::vector<helix2d::Module*> helix2d::Module::getAllModule()
{
	return allMod;
}

helix2d::Vector2 helix2d::Module::getTopLeftPos(const Painter* painter) const
{
	return painter->getTopLeftPos();
}

helix2d::Gravity::Gravity(float g)
{
	speedY = 0.0f;
	gravity = g;
}

void helix2d::Gravity::update(float delta)
{
	helix2d::Vector2&& pos = parent->getPos();
	speedY += gravity;
	if (speedY > 9.8f)
	{
		speedY = 9.8f;
	}
	pos.y += speedY;

	parent->setPos(pos);
}

helix2d::Collision::Collision()
{
	bEnableCollision = true;
}

void helix2d::Collision::enableCollision(bool b)
{
	bEnableCollision = b;
}

void helix2d::Collision::update(float delta)
{
	if (!bEnableCollision)
	{
		return;
	}

	if (parent->getWindow() == nullptr)
	{
		return;
	}

	
	lastTopLeftPos = getTopLeftPos(parent);

	for (auto& mod : getAllModule())
	{
		if (mod->getParent() == parent)
		{
			continue;
		}

		if (mod->getParent()->getWindow() == parent->getWindow())
		{
			repulsion(mod->getParent());
		}
	}
}

void helix2d::Collision::repulsion(Painter* painter)
{
	
}