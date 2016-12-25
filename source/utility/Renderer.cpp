#pragma once

#include "Renderer.h"
#include "Utility.hpp"
#include <string.h>
#include <vector>
#include <string>

Renderer::Renderer()
{
	// GDI+初期化
	Gdiplus::GdiplusStartup(&token_, &gpsi_, NULL);
}

Renderer::~Renderer()
{
	fonts_.clear();
	graphics_.reset();
	Gdiplus::GdiplusShutdown(token_);
}

/*!
 * @brief 初期化
 */
void Renderer::Initialize(HDC hdc)
{
	graphics_.reset(new Gdiplus::Graphics(hdc));
}

/*!
 * @brief 画面クリア
 */
void Renderer::ClearScreen(const Gdiplus::Color& color)
{
	graphics_->Clear(color);
}

/*!
 * @brief 塗りつぶし描画
 */
void Renderer::FillRect(int x, int y, int w, int h, const Gdiplus::Color& color)
{
	Gdiplus::SolidBrush brush(color);
	graphics_->FillRectangle(&brush, x, y, w, h);
}

/*!
 * @brief 画像描画
 */
void Renderer::DrawImage(Gdiplus::Bitmap* image, Anchor anchor, int x, int y)
{
	// アンカーを基準にして表示座標を変える
	x -= image->GetWidth() / 2 * (anchor % 3);
	y -= image->GetHeight() / 2 * (anchor / 3);

	// 描画
	graphics_->DrawImage(image, x, y);
}

/*!
 * @brief 画像描画
 */
void Renderer::DrawImage(Gdiplus::Bitmap* image, Anchor anchor, int dx, int dy, int dw, int dh,
	int sx, int sy, int sw, int sh, const Gdiplus::Color& color)
{
	if (color.GetA() == 0) return ;

	// 色設定
	Gdiplus::ImageAttributes ia;
	Gdiplus::ColorMatrix cm;
	memset(cm.m, 0, sizeof(cm.m));
	cm.m[0][0] = color.GetR() / 255.0f;
	cm.m[1][1] = color.GetG() / 255.0f;
	cm.m[2][2] = color.GetB() / 255.0f;
	cm.m[3][3] = color.GetA() / 255.0f;
	cm.m[4][4] = 1.0f;

	ia.SetColorMatrix(&cm);

	// アンカーを基準にして表示座標を変える
	dx -= dw / 2 * (anchor % 3);
	dy -= dh / 2 * (anchor / 3);

	// 描画
	Gdiplus::Rect dst(dx, dy, dw, dh);
	graphics_->DrawImage(image, dst, sx, sy, sw, sh, Gdiplus::UnitPixel, & ia);
}

/*!
 * @brief 文字列描画
 */
void Renderer::DrawString(const char* s, int x, int y, int size, const Gdiplus::Color& color)
{
	if (color.GetA() == 0) return ;

	this->DrawString(LEFT_TOP, x, y, size, color, s);
}

/*!
 * @brief 文字描画
 */
void Renderer::DrawString(Anchor anchor, int x, int y, int size, const Gdiplus::Color& color, const char* s, ...)
{
	if (color.GetA() == 0) return ;

	DrawString(anchor, x, y, size, color, Utility::SJIStoUTF16(s).c_str());
}

/*!
 * @brief 文字描画
 */
void Renderer::DrawString(Anchor anchor, int x, int y, int size, const Gdiplus::Color& color, const wchar_t* s, ...)
{
	if (color.GetA() == 0) return ;

	Gdiplus::Font* font = GetFont(size);
	Gdiplus::SolidBrush brush(color);
	Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoClip);
	Gdiplus::RectF range;

	// テキストフォーマットを変換
	wchar_t text[1024];
	va_list arg;
	va_start(arg, s);
	vswprintf(text, s, arg);
	va_end(arg);

	graphics_->MeasureString(text, wcslen(text), font, Gdiplus::PointF(), &format, &range);
	// アンカーを基準にして表示座標を変える
	x -= static_cast<int>(range.Width)  / 2 * (anchor % 3);
	y -= static_cast<int>(range.Height) / 2 * (anchor / 3);

	graphics_->DrawString(text, wcslen(text), font, Gdiplus::PointF(static_cast<float>(x), static_cast<float>(y)), &format, &brush);
}

/*!
 * @brief フォントの取得
 */
Gdiplus::Font* Renderer::GetFont(int size)
{
	auto it = fonts_.find(size);
	if (it != fonts_.end()) {
		return fonts_[size].get();
	}
	auto p = new Gdiplus::Font(L"ＭＳ ゴシック" , static_cast<Gdiplus::REAL>(size));
	fonts_.insert(std::pair<int, std::shared_ptr<Gdiplus::Font>>(size, p));
	return p;
}
