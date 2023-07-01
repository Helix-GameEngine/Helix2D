#include "h2dImage.h"
#include <fstream>
using namespace helix2d::Inbuilt;

std::map<std::wstring, helix2d::Image*> helix2d::Image::imgList;
std::map<helix2d::Image*, std::pair<ID2D1Bitmap*, bool>> 
	helix2d::Image::D2DimgList;

helix2d::Image::Image()
{
	pWICimage = nullptr;
	pConverter = nullptr;
}

helix2d::Image::~Image()
{
	SafeRelease(pWICimage);
	SafeRelease(pConverter);
}

std::wstring helix2d::Image::getPath() const
{
	return imgPath;
}

bool helix2d::Image::Load(std::wstring path)
{
	if (imgList[path] != nullptr)
	{
		return true;
	}

	std::fstream imgfile{path};
	if (imgfile.is_open())
	{
		imgList[path] = new Image;

		if (QueueMessage::mtx_msgPeek.try_lock())
		{
			QueueMessage::msgPeek.push_back(
				[&, path]() {
					auto pImage = imgList[path];

					IWICBitmapDecoder* pDecoder = nullptr;
					IWICBitmapFrameDecode* pWICimage = nullptr;

					auto hr = Renderer::pWICFactory->CreateDecoderFromFilename(
						path.c_str(),
						NULL,
						GENERIC_READ,
						WICDecodeMetadataCacheOnDemand,
						&pDecoder);

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
				}
			);
			QueueMessage::mtx_msgPeek.unlock();
		}

		return true;
	}
	return false;
}

void helix2d::Image::uninit()
{
	for (auto& prprBitmap : D2DimgList)
	{
		prprBitmap.second.second = false;
		SafeRelease(prprBitmap.second.first);
	}

	for (auto& prImage : imgList)
	{
		delete prImage.second;
	}
	imgList.erase(imgList.begin(), imgList.end());
}
