#pragma once
#include <d2d1.h>
#include <wincodec.h>
#include <dwrite.h>
#include <dwrite_3.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#pragma comment(lib, "windowscodecs.lib")

namespace helix2d
{
	class Window;
	class Base;

	class Renderer;
	class Painter;
	class Color;

	class Circle;

	class Module;
}

template <typename resource>
static void SafeRelease(resource*& p)
{
	if (p != nullptr)
	{
		p->Release();
		p = nullptr;
	}
}

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#	define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif