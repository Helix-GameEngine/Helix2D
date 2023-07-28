#pragma once
#include "h2dMacros.h"
#include "h2dRender.h"
#include <string>

namespace helix2d
{
	//字体
	class Font
	{
	public:

		Font();

		Font(const std::wstring& fontFamily, size_t fontSize = 20);
	public:

		//获取字体族
		std::wstring getFontFamily()const;

		//获取字号
		size_t getFontSize()const;
	private:

		std::wstring fontFamily;

		size_t fontSize;
	};

	//文本
	class Text :
		public Painter
	{
	public:

		Text();

		Text(const std::wstring& text, Font font = Font{});
	public:

		//设置显示文本
		void setText(std::wstring text);

		//设置字体
		void setFont(Font font);
	public:

		//获取显示文本
		std::wstring getText()const;

		//获取字体
		Font getFont()const;
	public:

		void Render() override;
	private:

		//重新创建资源
		void recreateSources();
	private:

		std::wstring text;

		Font font;

		IDWriteTextFormat* textFormat;

		IDWriteTextLayout* textLayout;
	};
}