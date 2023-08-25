#include "h2dImage.h"
#include <fstream>
using namespace helix2d::Inbuilt;

std::map<std::wstring, helix2d::Image*> helix2d::Image::imgList;
std::map<helix2d::Image*, std::pair<ID2D1Bitmap*, bool>> 
	helix2d::Image::D2DimgList;

helix2d::Image::Image()
{
	pConverter = nullptr;
}

helix2d::Image::~Image()
{
	SafeRelease(pConverter);
}

std::wstring helix2d::Image::getPath() const
{
	return imgPath;
}

bool helix2d::Image::load(std::wstring path)
{
	if (imgList.count(path))
	{
		return true;
	}

	std::fstream imgfile{path};
	if (imgfile.is_open())
	{
		imgfile.close();
		
		auto pImage = new Image;
		imgList[path] = pImage;

		IWICBitmapDecoder* pDecoder = nullptr;
		IWICBitmapFrameDecode* pWICimage = nullptr;

		auto hr = Renderer::pWICFactory->CreateDecoderFromFilename(
			path.c_str(),
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pDecoder
		);

		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, &pWICimage);
		}

		if (SUCCEEDED(hr))
		{
			hr = Renderer::pWICFactory->CreateFormatConverter(
				&pImage->pConverter);
		}


		if (SUCCEEDED(hr))
		{
			//Initialize Converter
			hr = pImage->pConverter->Initialize(
				pWICimage,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom
			);
		}

		if (SUCCEEDED(hr))
		{
			imgList[path] = pImage;
		}
		else
		{
			delete pImage;
			pImage = nullptr;
		}

		SafeRelease(pDecoder);
		SafeRelease(pWICimage);

		return true;
	}
	return false;
}

void helix2d::Image::uninit()
{
	if (!D2DimgList.empty())
	{
		for (auto it = D2DimgList.begin(); it != D2DimgList.end(); it++)
		{
			auto& prprBitmap = *it;
			prprBitmap.second.second = false;
			SafeRelease(prprBitmap.second.first);
		}
		D2DimgList.clear();
	}

	if (!imgList.empty())
	{
		for (auto it = imgList.begin(); it != imgList.end(); it++)
		{
			auto& prImage = *it;
			delete prImage.second;
			prImage.second = nullptr;
		}
		imgList.clear();
	}
}
