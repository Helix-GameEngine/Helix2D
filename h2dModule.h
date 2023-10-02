#pragma once
#include "h2dMacros.h"
#include "h2dTool.h"
#include <vector>
#include <map>

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

		//渲染
		virtual void BeforeRender() {};

		//子Painter渲染后
		virtual void AfterChildRender() {};
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

	//遮罩
	class Mask :
		public Module
	{
	public:

		Mask();
		Mask(float width, float height);

		~Mask();
	public:

		//设置实际宽度
		void setRealWidth(float width);
		//设置实际宽度
		void setRealHeight(float height);

		//设置是否反转
		void setInverse(bool inverse);

		//设置相对于Painter左上角的坐标
		void setPos(Vector2 pos);
		void setPos(float x, float y);
	public:

		//获取相对于Painter左上角的坐标
		Vector2 getPos();
	private:

		void BeforeRender() override;

		void AfterChildRender() override;
	private:

		ID2D1Layer* pLayer;
		ID2D1GeometrySink* pGeoSink;
		ID2D1PathGeometry* pPathGeo;

		bool isInverse;

		float width;
		float height;

		Vector2 pos;
	private:

		static ID2D1Geometry* currGeometry;

		static std::map<Painter*, ID2D1Geometry*> lastGeometries;

		static bool isPushLayer;
	};
}