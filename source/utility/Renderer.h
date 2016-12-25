#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <memory>
#include "master/MasterData.hpp"

enum Anchor
{
	LEFT_TOP,
	TOP,
	RIGHT_TOP,
	LEFT,
	CENTER,
	RIGHT,
	LEFT_BOTTOM,
	BOTTOM,
	RIGHT_BOTTOM,
};

class Renderer
{
public :
	~Renderer();

	/*!
	 * @brief ‰Šú‰»
	 */
	void Initialize(HDC hdc);

	/*!
	 * @brief ‰æ–ÊƒNƒŠƒA
	 */
	void ClearScreen(const Gdiplus::Color& color);

	/*!
	 * @brief “h‚è‚Â‚Ô‚µ•`‰æ
	 */
	void FillRect(int x, int y, int w, int h, const Gdiplus::Color& color);

	/*!
	 * @brief ‰æ‘œ•`‰æ
	 */
	void DrawImage(Gdiplus::Bitmap* image, Anchor anchor, int x, int y);

	/*!
	 * @brief ‰æ‘œ•`‰æ
	 */
	void DrawImage(Gdiplus::Bitmap* image, Anchor anchor, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, const Gdiplus::Color& color);

	/*!
	 * @brief •¶š—ñ•`‰æ
	 */
	void DrawString(const char* s, int x, int y, int size = 20, const Gdiplus::Color& color = Gdiplus::Color::White);

	/*!
	 * @brief •¶š—ñ•`‰æ
	 */
	void DrawString(Anchor anchor, int x, int y, int size, const Gdiplus::Color& color, const char* s, ...);

	/*!
	 * @brief •¶š—ñ•`‰æ
	 */
	void DrawString(Anchor anchor, int x, int y, int size, const Gdiplus::Color& color, const wchar_t* s, ...);

	/*!
	 * @brief ƒCƒ“ƒXƒ^ƒ“ƒXæ“¾
	 */
	static Renderer* GetInstance() {
		static Renderer v;
		return &v;
	}

private :
	Renderer();
	Gdiplus::Font* GetFont(int size);

	Gdiplus::GdiplusStartupInput gpsi_;
	ULONG_PTR token_;
	std::shared_ptr<Gdiplus::Graphics> graphics_;
	std::map<int, std::shared_ptr<Gdiplus::Font>> fonts_;
	std::shared_ptr<Gdiplus::SolidBrush> brush_;
	Gdiplus::ImageAttributes ia_;
	Gdiplus::ColorMatrix cm_;
	Gdiplus::StringFormat string_format_;
};
