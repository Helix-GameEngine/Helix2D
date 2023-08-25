#pragma once
#include <d2d1.h>
#include <wincodec.h>
#include <dwrite.h>
#include <dwrite_3.h>
#include <dwmapi.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "dwmapi.lib")

#include <vector>

//#ifndef _LIB_HELIX2D_
//#define _LIB_HELIX2D_
//#pragma comment(lib,"libhelix2d")
//#endif // !_LIB_HELIX2D_

template <typename resource>
static void SafeRelease(resource*& p)
{
	if (p != nullptr)
	{
		p->Release();
		p = nullptr;
	}
}

template <typename T>
static bool SafeRemove(std::vector<T>& container, T& item)
{
	auto it = std::find(container.begin(), container.end(), item);
	if (it != container.end())
	{
		container.erase(it);
		return true;
	}
	return false;
}

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#ifndef HINST_THISCOMPONENT
#	define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif