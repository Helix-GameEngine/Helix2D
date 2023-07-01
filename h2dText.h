#pragma once
#include "h2dMacros.h"
#include "h2dRender.h"
#include <string>

namespace helix2d
{
	class Font
	{
	public:

		Font();

		Font(const std::wstring& fontFamily, size_t fontSize = 20);
	public:

		std::wstring getFontFamily()const;

		size_t getFontSize()const;
	private:

		std::wstring fontFamily;

		size_t fontSize;
	};

	class Text :
		public Painter
	{
	public:

		Text();

		Text(const std::wstring& text, Font font = Font{});
	public:

		void setText(std::wstring text);

		void setFont(Font font);
	public:

		std::wstring getText()const;
	public:

		void Render() override;
	private:

		void recreateSources();
	private:

		std::wstring text;

		Font font;

		IDWriteTextFormat* textFormat;

		IDWriteTextLayout* textLayout;
	};
}