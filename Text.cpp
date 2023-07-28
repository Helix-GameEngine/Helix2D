#include "h2dText.h"
#include "h2dBase.h"

helix2d::Font::Font()
{
	fontSize = 24;
}

helix2d::Font::Font(const std::wstring& fontFamily, size_t fontSize)
{
	this->fontSize = fontSize;
	this->fontFamily = fontFamily;
}

std::wstring helix2d::Font::getFontFamily() const
{
	return fontFamily;
}

size_t helix2d::Font::getFontSize() const
{
	return fontSize;
}

helix2d::Text::Text()
{
	textFormat = nullptr;
	textLayout = nullptr;
}

helix2d::Text::Text(const std::wstring& text, Font font)
{
	textLayout = nullptr;
	textFormat = nullptr;
	setText(text);
	setFont(font);
}

void helix2d::Text::setText(std::wstring text)
{
	this->text = text;

	recreateSources();
}

void helix2d::Text::setFont(Font font)
{
	this->font = font;

	recreateSources();
}

std::wstring helix2d::Text::getText() const
{
	return text;
}

helix2d::Font helix2d::Text::getFont() const
{
	return font;
}

void helix2d::Text::Render()
{
	if (text.empty())
	{
		return;
	}

	if (textFormat == nullptr)
	{
		return;
	}

	if (textLayout == nullptr)
	{
		return;
	}

	DWRITE_TEXT_METRICS TextMetrics{};
	textLayout->GetMetrics(&TextMetrics);

	realWidth = TextMetrics.width;
	realHeight = TextMetrics.height;

	auto& pTarget = window->getRenderer()->pD2D1RenderTarget;
	auto& pBrush = window->getRenderer()->pD2D1SolidBrush;

	auto rect = D2D1::RectF(
		getUpperleftPosX(), getUpperleftPosY(),
		getLowerrightPosX(),getLowerrightPosY());

	pTarget->DrawTextW(
		text.c_str(),
		static_cast<UINT32>(text.size()),
		textFormat,
		rect,
		pBrush
	);
}

void helix2d::Text::recreateSources()
{
	SafeRelease(textFormat);
	SafeRelease(textLayout);

	auto hr = Renderer::pDWriteFactory->CreateTextFormat(
		font.getFontFamily().c_str(),
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		static_cast<float>(font.getFontSize()),
		L"",
		&textFormat
	);

	if (SUCCEEDED(hr))
	{
		hr = Renderer::pDWriteFactory->CreateTextLayout(
			text.c_str(),
			static_cast<UINT32>(text.size()),
			textFormat,
			1000.0f,
			50.0f,
			&textLayout
		);
	}

	if (SUCCEEDED(hr))
	{
		DWRITE_TEXT_METRICS TextMetrics{};
		textLayout->GetMetrics(&TextMetrics);

		realWidth = TextMetrics.width;
		realHeight = TextMetrics.height;
	}

	if (FAILED(hr))
	{
		SafeRelease(textFormat);
		SafeRelease(textLayout);
		return;
	}
}

