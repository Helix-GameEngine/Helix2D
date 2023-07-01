#pragma once
#include "h2dMacros.h"

namespace helix2d
{
	enum class KeyCode;
	enum class MouseCode;

	class Vector2
	{
	public:

		Vector2();

		Vector2(const D2D1_POINT_2F& point);

		Vector2(float x, float y);
	public:

		Vector2 operator+(const Vector2& vec)const;

		Vector2 operator-(const Vector2& vec)const;

		Vector2 operator=(const Vector2& vec);

		Vector2 operator+=(const Vector2& vec);

		Vector2 operator-=(const Vector2& vec);

		Vector2 operator*(float num)const;

		Vector2 operator*=(float num);

		operator D2D1_POINT_2F();
	public:

		float dot(const Vector2& vec);

		Vector2 cross(const Vector2& vec);
	public:

		float x;
		float y;
	};

	using Scale = Vector2;

	using Matrix3x2 = D2D1::Matrix3x2F;

	class Input
	{
	public:

		static bool isDown(Window* window, const KeyCode& key, float downTime = 0.0f);
		static bool isDown(Window* window, const MouseCode& key, float downTime = 0.0f);

		static bool isRelease(Window* window, const KeyCode& key);
		static bool isRelease(Window* window, const MouseCode& key);

		static bool isUpPress(Window* window, const KeyCode& key, float downTime = 0.0f);
		static bool isUpPress(Window* window, const MouseCode& key, float downTime = 0.0f);

		static bool isDownPress(Window* window, const KeyCode& key);
		static bool isDownPress(Window* window, const MouseCode& key);
	};

	enum class MouseCode
	{
		Left,
		Mid,
		Right
	};

	enum class KeyCode
	{
		Unknown = 0,
		Up = VK_UP,
		Left = VK_LEFT,
		Right = VK_RIGHT,
		Down = VK_DOWN,
		Enter = VK_RETURN,
		Space = VK_SPACE,
		Esc = VK_ESCAPE,
		LCtrl = VK_LCONTROL,
		RCtrl = VK_RCONTROL,
		LShift = VK_LSHIFT,
		RShift = VK_RSHIFT,
		LAlt = VK_LMENU,
		RAlt = VK_RMENU,
		Tab = VK_TAB,
		Delete = VK_DELETE,
		Back = VK_BACK,
		Home = VK_HOME,
		End = VK_END,
		Insert = VK_INSERT,
		PageUp = VK_PRIOR,
		PageDown = VK_NEXT,

		A = 0x41,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		Num0 = 0x30,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,

		Numpad0 = VK_NUMPAD0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,

		NumpadAdd = VK_ADD,
		NumpadMinus = VK_SUBTRACT,
		NumpadMultiply = VK_MULTIPLY,
		NumpadDivide = VK_DIVIDE,
		NumpadDot = VK_DECIMAL,

		F1 = VK_F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,

		Add = VK_OEM_PLUS,
		Minus = VK_OEM_MINUS,
		Comma = VK_OEM_COMMA,
		Dot = VK_OEM_PERIOD,
		Backslash = VK_OEM_5,
		Slash = VK_OEM_2,
		Semicolon = VK_OEM_1,
		LSquareBrackets = VK_OEM_4,
		RSquareBrackets = VK_OEM_6,
		Apostrophe = VK_OEM_7,
		Tilde = VK_OEM_3,
	};
}
