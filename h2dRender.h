#pragma once
#include "h2dMacros.h"
#include "h2dTool.h"
#include <vector>

namespace helix2d
{
	class Window;
	class Module;

	class Renderer 
	{
		friend class Window;
	private:

		explicit Renderer(Window* window);

		Renderer();
	public:

		bool createDeviceResources();
		void discardDeviceResources();
	public:

		static bool createDeviceIndependentResources();
		static void discardResources();
	private:

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

	class Color
	{
	public:

		Color();

		Color(float r, float g, float b, float a = 1.0f);

		Color(long rgb, float a = 1.0f);
	public:

		void setColor(float r, float g, float b, float a = 1.0f);

		void setColor(long rgb, float a = 1.0f);
	public:

		operator D2D1_COLOR_F();
	public:

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

		float r;
		float g;
		float b;
		float a;
	};

	class Painter
	{
		friend class Window;
		friend class Renderer;
		friend class Module;
	public:

		Painter();
	public:

		void addPainter(Painter* pPainter);
		void addModule(Module* pMod);
	public:

		void setPos(float x, float y);
		void setPos(Vector2 pos);

		void setPosX(float x);
		void setPosY(float y);

		void setScale(Scale scale);
		void setScale(float scaleX, float scaleY);

		void setAnchor(Vector2 anchor);
		void setAnchor(float x, float y);

		void setAngle(float angle);

		void setColor(Color c);

		void setFlipX(bool flipX);
		void setFlipY(bool flipY);

		void enableTick(bool b);
	public:

		void movePos(float x, float y);
		void movePos(Vector2 pos);

		void movePosX(float x);
		void movePosY(float y);

		void rotate(float angle);

		void flipX();
		void flipY();
	public:

		Window* getWindow()const;

		Painter* getParent()const;

		std::vector<Painter*> getAllPainter()const;

		Vector2 getPos()const;

		float getPosX()const;
		float getPosY()const;

		float getWidth()const;
		float getHeight()const;

		Scale getScale()const;
		float getScaleX()const;
		float getScaleY()const;

		Vector2 getAnchor()const;

		float getAngle()const;

		Vector2 getWorldPos()const;

		Vector2 getRealPos()const;

		Vector2 getTopLeftPos()const;
		float getTopLeftPosX()const;
		float getTopLeftPosY()const;

		Vector2 getBottomRightPos()const;
		float getBottomRightPosX()const;
		float getBottomRightPosY()const;


		Vector2 getTopRightPos()const;
		float getTopRightPosX()const;
		float getTopRightPosY()const;

		Vector2 getBottomLeftPos()const;
		float getBottomLeftPosX()const;
		float getBottomLeftPosY()const;
	public:

		bool isOverlap(const Painter* other)const;

		bool isEnableTick()const;
	public:

		void Paint();
		void Paint(Vector2 pos, float angle);
		void Paint(Window* window);
		void Paint(Vector2 pos, float angle, Window* window);

		virtual void Tick(float delta) {};
	public:

		static void setDefaultAnchorPos(Vector2 pos);
		static void setDefaultAnchorPos(float x, float y);

		static void updateProperty(Window* window);
	protected:

		virtual void Render() {};
	private:

		void _render();

		void _render(Vector2 pos, float angle, Window* window);		

		void _update(float delta);

		void _updateModule(float delta);
	private:

		void setWindow(Window* window);

		void updateProperty();
	protected:

		float width;
		float height;

		float angle;

		Scale scale;

		Color color;
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
	private:

		Vector2 anchor;

		bool bEnableTick;
	private:

		std::vector<Painter*> painterList;
		std::vector<Module*> modList;
	private:

		static Vector2 defaultAnchor;
	};
}
