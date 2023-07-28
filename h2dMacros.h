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

#ifndef H2D_LIBHELIX2D
#define H2D_LIBHELIX2D
//#pragma comment(lib,"libhelix2d")
#endif // !H2D_LIBHELIX2D


#include <vector>


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
static void SafeRemove(std::vector<T>& container, T& item)
{
	auto it = std::find(container.begin(), container.end(), item);
	if (it != container.end())
	{
		container.erase(it);
	}
}

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#ifndef HINST_THISCOMPONENT
#	define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif