#pragma once
#include "h2dMacros.h"
#include "h2dTool.h"
#include <string>
#include <vector>

namespace helix2d
{
	class Window;
	class Module;

	//渲染器
	class Renderer 
	{
		friend class Window;
	private:

		explicit Renderer(Window* window);

		~Renderer();
	public:

		//创建设备相关资源
		bool createDeviceResources();
		//销毁设备相关资源
		void discardDeviceResources();
	public:

		//创建设备无关资源
		static bool createDeviceIndependentResources();
		//销毁设备无关资源
		static void discardResources();
	private:

		//渲染
		void _render(float delta);
	public:

		static ID2D1Factory* pD2D1Factory;

		static ID2D1StrokeStyle* pD2D1MiterStrokeStyle;
		static ID2D1StrokeStyle* pD2D1BevelStrokeStyle;
		static ID2D1StrokeStyle* pD2D1RoundStrokeStyle;

		static IWICImagingFactory* pWICFactory;

		static IDWriteFactory5* pDWriteFactory;

		static IDWriteTextFormat* pDWTextFormat;
	public:

		ID2D1HwndRenderTarget* pD2D1RenderTarget;
		ID2D1SolidColorBrush* pD2D1SolidBrush;

	private:

		float fDPIScaleX;
		float fDPIScaleY;

		bool bVSyncEnabled;
		bool bDeviceResourceRecreated;
	private:

		Window* window;
	};

	//颜色
	class Color
	{
	public:

		//颜色值
		enum 
		{
			AliceBlue = 0xF0F8FF,
			AntiqueWhite = 0xFAEBD7,
			Aqua = 0x00FFFF,
			Aquamarine = 0x7FFFD4,
			Azure = 0xF0FFFF,
			Beige = 0xF5F5DC,
			Bisque = 0xFFE4C4,
			Black = 0x000000,
			BlanchedAlmond = 0xFFEBCD,
			Blue = 0x0000FF,
			BlueViolet = 0x8A2BE2,
			Brown = 0xA52A2A,
			BurlyWood = 0xDEB887,
			CadetBlue = 0x5F9EA0,
			Chartreuse = 0x7FFF00,
			Chocolate = 0xD2691E,
			Coral = 0xFF7F50,
			CornflowerBlue = 0x6495ED,
			Cornsilk = 0xFFF8DC,
			Crimson = 0xDC143C,
			Cyan = 0x00FFFF,
			DarkBlue = 0x00008B,
			DarkCyan = 0x008B8B,
			DarkGoldenrod = 0xB8860B,
			DarkGray = 0xA9A9A9,
			DarkGreen = 0x006400,
			DarkKhaki = 0xBDB76B,
			DarkMagenta = 0x8B008B,
			DarkOliveGreen = 0x556B2F,
			DarkOrange = 0xFF8C00,
			DarkOrchid = 0x9932CC,
			DarkRed = 0x8B0000,
			DarkSalmon = 0xE9967A,
			DarkSeaGreen = 0x8FBC8F,
			DarkSlateBlue = 0x483D8B,
			DarkSlateGray = 0x2F4F4F,
			DarkTurquoise = 0x00CED1,
			DarkViolet = 0x9400D3,
			DeepPink = 0xFF1493,
			DeepSkyBlue = 0x00BFFF,
			DimGray = 0x696969,
			DodgerBlue = 0x1E90FF,
			Firebrick = 0xB22222,
			FloralWhite = 0xFFFAF0,
			ForestGreen = 0x228B22,
			Fuchsia = 0xFF00FF,
			Gainsboro = 0xDCDCDC,
			GhostWhite = 0xF8F8FF,
			Gold = 0xFFD700,
			Goldenrod = 0xDAA520,
			Gray = 0x808080,
			Green = 0x008000,
			GreenYellow = 0xADFF2F,
			Honeydew = 0xF0FFF0,
			HotPink = 0xFF69B4,
			IndianRed = 0xCD5C5C,
			Indigo = 0x4B0082,
			Ivory = 0xFFFFF0,
			Khaki = 0xF0E68C,
			Lavender = 0xE6E6FA,
			LavenderBlush = 0xFFF0F5,
			LawnGreen = 0x7CFC00,
			LemonChiffon = 0xFFFACD,
			LightBlue = 0xADD8E6,
			LightCoral = 0xF08080,
			LightCyan = 0xE0FFFF,
			LightGoldenrodYellow = 0xFAFAD2,
			LightGreen = 0x90EE90,
			LightGray = 0xD3D3D3,
			LightPink = 0xFFB6C1,
			LightSalmon = 0xFFA07A,
			LightSeaGreen = 0x20B2AA,
			LightSkyBlue = 0x87CEFA,
			LightSlateGray = 0x778899,
			LightSteelBlue = 0xB0C4DE,
			LightYellow = 0xFFFFE0,
			Lime = 0x00FF00,
			LimeGreen = 0x32CD32,
			Linen = 0xFAF0E6,
			Magenta = 0xFF00FF,
			Maroon = 0x800000,
			MediumAquamarine = 0x66CDAA,
			MediumBlue = 0x0000CD,
			MediumOrchid = 0xBA55D3,
			MediumPurple = 0x9370DB,
			MediumSeaGreen = 0x3CB371,
			MediumSlateBlue = 0x7B68EE,
			MediumSpringGreen = 0x00FA9A,
			MediumTurquoise = 0x48D1CC,
			MediumVioletRed = 0xC71585,
			MidnightBlue = 0x191970,
			MintCream = 0xF5FFFA,
			MistyRose = 0xFFE4E1,
			Moccasin = 0xFFE4B5,
			NavajoWhite = 0xFFDEAD,
			Navy = 0x000080,
			OldLace = 0xFDF5E6,
			Olive = 0x808000,
			OliveDrab = 0x6B8E23,
			Orange = 0xFFA500,
			OrangeRed = 0xFF4500,
			Orchid = 0xDA70D6,
			PaleGoldenrod = 0xEEE8AA,
			PaleGreen = 0x98FB98,
			PaleTurquoise = 0xAFEEEE,
			PaleVioletRed = 0xDB7093,
			PapayaWhip = 0xFFEFD5,
			PeachPuff = 0xFFDAB9,
			Peru = 0xCD853F,
			Pink = 0xFFC0CB,
			Plum = 0xDDA0DD,
			PowderBlue = 0xB0E0E6,
			Purple = 0x800080,
			Red = 0xFF0000,
			RosyBrown = 0xBC8F8F,
			RoyalBlue = 0x4169E1,
			SaddleBrown = 0x8B4513,
			Salmon = 0xFA8072,
			SandyBrown = 0xF4A460,
			SeaGreen = 0x2E8B57,
			SeaShell = 0xFFF5EE,
			Sienna = 0xA0522D,
			Silver = 0xC0C0C0,
			SkyBlue = 0x87CEEB,
			SlateBlue = 0x6A5ACD,
			SlateGray = 0x708090,
			Snow = 0xFFFAFA,
			SpringGreen = 0x00FF7F,
			SteelBlue = 0x4682B4,
			Tan = 0xD2B48C,
			Teal = 0x008080,
			Thistle = 0xD8BFD8,
			Tomato = 0xFF6347,
			Turquoise = 0x40E0D0,
			Violet = 0xEE82EE,
			Wheat = 0xF5DEB3,
			White = 0xFFFFFF,
			WhiteSmoke = 0xF5F5F5,
			Yellow = 0xFFFF00,
			YellowGreen = 0x9ACD32,
		};
	public:

		Color();

		Color(float r, float g, float b, float a = 1.0f);

		Color(long rgb, float a = 1.0f);
	public:

		//设置颜色值
		void setColor(float r, float g, float b, float a = 1.0f);
		void setColor(long rgb, float a = 1.0f);
	public:

		operator D2D1_COLOR_F();
	public:

		
	public:

		float r;
		float g;
		float b;
		float a;
	};

	//画家
	class Painter
	{
		friend class Window;
		friend class Renderer;
	public:

		Painter();
	public:

		//添加子画家
		void addPainter(Painter* pPainter);
		void addPainter(Painter* pPainter, std::wstring name);

		//添加模块
		void addModule(Module* pMod);

		//移除子画家
		bool removePainter(Painter* pPainter);

		//移除模块
		bool removeModule(Module* pMod);

		//从子画家中查找画家
		Painter* findPainter(std::wstring name);
	public:

		//设置坐标
		void setPos(float x, float y);
		void setPos(Vector2 pos);

		//设置X轴坐标
		void setPosX(float x);
		//设置Y轴坐标
		void setPosY(float y);

		//设置缩放
		void setScale(Scale scale);
		void setScale(float scaleX, float scaleY);

		//设置锚点
		void setAnchor(Vector2 anchor);
		void setAnchor(float x, float y);

		//设置旋转角度
		void setAngle(float angle);

		//设置颜色
		void setColor(Color c);

		//设置是否X轴翻转（上下翻转）
		void setFlipX(bool flipX);
		//设置是否Y轴翻转（左右翻转）
		void setFlipY(bool flipY);

		//设置速度
		void setVelocity(Vector2 v);
		void setVelocity(float vx, float vy);
		//设置X轴速度
		void setVelocityX(float vx);
		//设置Y轴速度
		void setVelocityY(float vy);

		//设置质量
		void setMass(float m);

		//设置渲染顺序
		void setOrder(size_t nOrder);

		//设置名字
		void setName(std::wstring name);

		//设置是否启用Tick方法
		void enableTick(bool b);
		//设置是否启用Render方法
		void enableRender(bool b);
		//设置是否更新
		void enableUpdate(bool b);
	public:

		//移动坐标
		void movePos(float x, float y);
		void movePos(Vector2 pos);

		//移动X轴坐标
		void movePosX(float x);
		//移动Y轴坐标
		void movePosY(float y);

		//以当前方向前进
		void moveAhead(float size);
		//以指定方向前进
		void moveAhead(float size, float angle);

		//旋转
		void rotate(float angle);

		//X轴翻转（上下翻转）
		void flipX();
		//Y轴翻转（左右翻转）
		void flipY();

		//增加速度
		void addVelocity(Vector2 a);
		void addVelocity(float ax, float ay);
		//增加X轴速度
		void addVelocityX(float a);
		//增加Y轴速度
		void addVelocityY(float a);

		//以当前方向加速
		void addVelocityAhead(float size);
		//以指定方向加速
		void addVelocityAhead(float size, float angle);
	public:

		//获取画家所在窗口
		Window* getWindow()const;

		//获取父画家
		Painter* getParent()const;

		//获取所有子画家
		std::vector<Painter*> getAllPainter()const;
		//获取所有已添加模块
		std::vector<Module*> getAllModule()const;

		//获取坐标
		Vector2 getPos()const;
		//获取X轴坐标
		float getPosX()const;
		//获取Y轴坐标
		float getPosY()const;

		//获取实际宽度
		float getRealWidth()const;
		//获取实际高度
		float getRealHeight()const;

		//获取缩放
		Scale getScale()const;
		//获取X轴缩放
		float getScaleX()const;
		//获取Y轴缩放
		float getScaleY()const;

		//获取锚点
		Vector2 getAnchor()const;

		//获取旋转角度
		float getAngle()const;

		//获取世界坐标
		Vector2 getWorldPos()const;

		//获取实际坐标
		Vector2 getRealPos()const;

		//获取左上角坐标（显示大小下）
		Vector2 getDisplayUpperleftPos()const;

		//获取左上角坐标（原始大小下）
		Vector2 getUpperleftPos()const;
		//获取左上角X坐标（原始大小下）
		float getUpperleftPosX()const;
		//获取左上角Y坐标（原始大小下）
		float getUpperleftPosY()const;

		//获取右下角坐标（原始大小下）
		Vector2 getLowerrightPos()const;
		//获取右下角X坐标（原始大小下）
		float getLowerrightPosX()const;
		//获取右下角Y坐标（原始大小下）
		float getLowerrightPosY()const;

		//获取右上角坐标（原始大小下）
		Vector2 getUpperrightPos()const;
		//获取右上角X坐标（原始大小下）
		float getUpperrightPosX()const;
		//获取右上角Y坐标（原始大小下）
		float getUpperrightPosY()const;

		//获取左下角坐标（原始大小下）
		Vector2 getLowerleftPos()const;
		//获取左下角X坐标（原始大小下）
		float getLowerleftPosX()const;
		//获取左下角Y坐标（原始大小下）
		float getLowerleftPosY()const;

		//获取速度
		Vector2 getVelocity()const;
		//获取X轴速度
		float getVelocityX()const;
		//获取Y轴速度
		float getVelocityY()const;

		//获取质量
		float getMass()const;

		//获取渲染顺序
		size_t getOrder()const;

		//获取名字
		std::wstring getName()const;
	public:

		//是否与其他画家重叠
		bool isOverlap(const Painter* other)const;

		//是否启用Tick方法
		bool isEnableTick()const;
		//是否启用Render方法
		bool isEnableRender()const;
		//是否更新
		bool isEnableUpdate()const;
	public:

		//绘制图像
		void paint();
		void paint(Vector2 pos, float angle);
		void paint(Window* window);
		void paint(Vector2 pos, float angle, Window* window);
	public:

		//每帧调用一次
		virtual void Tick(float delta) {};
	public:

		//设置默认锚点
		static void setDefaultAnchor(Vector2 pos);
		static void setDefaultAnchor(float x, float y);

		//更新属性
		static void updateProperty(Window* window);

		//更新Tick
		static void updateTick(Window* window, float delta);

		//更新渲染
		static void updateRender(Window* window);

		//每帧最后更新一次
		static void updateEnd(Window* window, float delta);
	protected:

		//渲染
		virtual void Render() {};

		//在子画家渲染之后调用
		virtual void AfterChildRender() {};

		//在更新属性时调用
		virtual void UpdateProperty() {};
	private:

		void _render();

		void _render(Vector2 pos, float angle, Window* window);		

		void _update(float delta);

		void _updateModule(float delta);
	private:

		void setWindow(Window* window);

		void updateProperty();
		void updateEnd(float delta);
		
		//模块排序
		void sortModule();
		//渲染排序
		void sortOrder();
	protected:

		//显示宽度
		float width;
		//显示高度
		float height;

		//实际宽度
		float realWidth;
		//实际高度
		float realHeight;

		float angle;
		float realAngle;

		Scale scale;

		Color color;

		size_t order;

		std::wstring name;
	protected:

		Vector2 velocity;

		float mass;
	protected:

		Window* window;

		Painter* parent;
	private:

		float fFlipX;
		float fFlipY;
	private:

		//相对坐标
		Vector2 pos;

		//世界坐标
		Vector2 worldPos;

		//实际坐标
		Vector2 realPos;

		//旋转矩阵
		Matrix3x2 rotationMatrix;

		//实际缩放
		Scale realScale;
	private:

		Vector2 anchor;

		bool bEnableTick;
		bool bEnableRender;
		bool bEnableUpdate;

		bool bSortModule;

		bool bSortOrder;
	private:

		std::vector<Painter*> painterList;
		std::vector<Module*> modList;
	private:

		static Vector2 defaultAnchor;
	};
}
