#pragma once
#include "h2dMacros.h"
#include "h2dBase.h"
#include <string>
#include <map>

namespace helix2d
{
	class Sprite;
	class Renderer;

	//图片
	class Image
	{
		friend class Sprite;
		friend class Renderer;
	public:

		Image();

		~Image();
	public:

		std::wstring getPath()const;
	public:

		static bool load(std::wstring path);
	public:

		static void uninit();
	private:

		std::wstring imgPath;

		IWICFormatConverter* pConverter;
	private:

		static std::map<std::wstring, Image*> imgList;

		static std::map<Image*, std::pair<ID2D1Bitmap*, bool>>
			D2DimgList;
	};

	//缩放模式
	enum class ScaleMode
	{
		//邻近缩放
		Neighbor = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		//线性缩放
		Linear = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
	};

	//精灵
	class Sprite :
		public Painter
	{
	public:

		Sprite();

		explicit Sprite(const std::wstring& path);
	public:

		//打开图片
		void open(const std::wstring& path);
	public:

		//设置缩放模式
		void setScaleMode(const ScaleMode& scaleMode);
	private:

		void Render() override;
	private:

		std::wstring imgPath;

		Image* img;
		
		ID2D1Bitmap* pD2DImg;

		ScaleMode scaleMode;
	};
}