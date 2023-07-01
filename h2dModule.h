#pragma once
#include "h2dMacros.h"
#include "h2dTool.h"
#include <vector>

namespace helix2d
{
	class Painter;

	class Module
	{
		friend class Painter;
	public:

		Module();

		~Module();
	public:

		Painter* getParent()const;
	public:

		static std::vector<Module*> getAllModule();
	protected:

		virtual void update(float delta) {};
	protected:

		Vector2 getTopLeftPos(const Painter* painter)const;
	protected:

		Painter* parent;
	private:

		static std::vector<Module*> allMod;
	};

	class Gravity :
		public Module
	{
	public:

		Gravity(float g = 0.1f);
	private:

		void update(float delta) override;
	private:
		
		float speedY;

		float gravity;
	};

	class Collision :
		public Module
	{
	public:

		Collision();
	public:

		void enableCollision(bool b);
	private:

		void update(float delta) override;
	private:

		void repulsion(Painter* painter);
	private:

		bool bEnableCollision;

		Vector2 lastTopLeftPos;
	};
}