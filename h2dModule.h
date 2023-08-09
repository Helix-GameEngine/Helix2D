#pragma once
#include "h2dMacros.h"
#include "h2dTool.h"
#include <vector>

namespace helix2d
{
	class Painter;
	class Window;

	//模块
	class Module
	{
		friend class Painter;
	public:

		enum class Type
		{
			//修改模块
			Modifier,
			//检查模块
			Detector
		};
	public:

		Module();

		~Module();
	public:

		//获取父画家
		Painter* getParent()const;

		//获取模块类型
		Module::Type getModuleType()const;
	public:

		//更新模块
		static void updateModule(Window* window, float delta);
	public:

		//获取所有模块
		static std::vector<Module*> getAllModule();
	protected:

		//更新
		virtual void Update(float delta) {};
	private:

		//更新特定类型模块
		static void updateTypeModule(Window* window, float delta, Type type);
	protected:

		Painter* parent;
	protected:

		Type modType;
	private:

		static std::vector<Module*> allMod;
	};

	//重力
	class Gravity :
		public Module
	{
	public:

		explicit Gravity(float g = 1.0f);
	public:

		//设置重力系数
		void setGravity(float g);
	public:

		//获取重力系数
		float getGravity()const;
	private:

		void Update(float delta) override;
	private:

		float gravity;
	};
}