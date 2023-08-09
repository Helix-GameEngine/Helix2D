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