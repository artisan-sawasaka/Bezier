#include "main.h"
#include "utility/Renderer.h"
#include "utility/Utility.hpp"
#include <set>
#include <algorithm>

/*!
 * @brief WinMain
 */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// クラス名とタイトルを設定
	MainApp app(_T("bezier"), _T("bezierのテスト"));

	// アプリをスタート
	app.Start();

	return 0;
}

MainApp::MainApp(TCHAR* wnClassName, TCHAR* title)
	: AppBase(wnClassName, title)
{
}

MainApp::~MainApp()
{
}

/*!
 * @brief 継承したウインドプロシージャ
 */
LRESULT MainApp::WndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*!
 * @brief 継承した初期化
 */
void MainApp::Initialize()
{
	// レンダラーを初期化
	Renderer::GetInstance()->Initialize(GetBackHDC());

	// マスターデータ読み込み
	Reload_();
	
	// 各種カウンター初期化
	stop_counter_.set(0, GetWidth() - 100, 6);
	root_counter_.set(0, GetWidth(), 4);
	fade_counter_.set(0, 0, 1);
	cursor_counter_.set(0, 10, -3);
	SetBezier_(Bezier::Linear);
}

/*!
 * @brief 継承した終了化
 */
void MainApp::Finalize()
{
}

/*!
 * @brief 継承したメインループ
 */
void MainApp::Update(float df)
{
	if (GetKeyState('R') < 0) {
		// マスターデータ再読み込み
		Reload_();
	} else if (GetKeyState('I') < 0) {
		// フェードイン
		fade_counter_.set(fade_counter_, 0, 30);
	} else if (GetKeyState('O') < 0) {
		// フェードアウト
		fade_counter_.set(fade_counter_, 255, 30);
	} else if (GetKeyState('S') < 0) {
		// 止まったカウンターを再度再生
		stop_counter_.set(0, GetWidth() - 100, 6);
	} else if (GetKeyState('1') < 0) {
		SetBezier_(Bezier::Linear);
	} else if (GetKeyState('2') < 0) {
		SetBezier_(Bezier::EaseIn);
	} else if (GetKeyState('3') < 0) {
		SetBezier_(Bezier::EaseOut);
	} else if (GetKeyState('4') < 0) {
		SetBezier_(Bezier::EaseInOut);
	} else if (GetKeyState('5') < 0) {
		SetBezier_(Bezier::EaseInBack);
	} else if (GetKeyState('6') < 0) {
		SetBezier_(Bezier::EaseOutBack);
	} else if (GetKeyState('7') < 0) {
		SetBezier_(Bezier::EaseInOutBack);
	} else if (GetKeyState('8') < 0) {
		SetBezier_(Bezier::EaseInCirc);
	}

	++stop_counter_;
	++root_counter_;
	++fade_counter_;
	++cursor_counter_;
	++bezier_counter_;
	bezier_timer_.Update(df);

	// とまる～を操作
	objects_["TextStop"]->x = stop_counter_;

	// まわる～を操作
	objects_["TextLoop"]->x = root_counter_;

	// カーソルを操作
	objects_["TextCursor"]->y = cursour_y_ + cursor_counter_;

	// ベジェカウンターを操作
	objects_["BezierCounter"]->x = bezier_counter_;

	// カーソルを操作
	objects_["BezierTimer"]->x = bezier_timer_;

	// 画面クリア
	Renderer::GetInstance()->ClearScreen(Gdiplus::Color(0, 80, 255));

	// マスターデータに基づく描画処理
	for (auto it = MasterData::TitleUI.begin(); it != MasterData::TitleUI.end(); ++it) {
		const auto& info = *it;
		if (info.type == 0) {
			// 画像描画
			auto it2 = MasterData::TitleImageList.find(info.name);
			if (it2 == MasterData::TitleImageList.end()) continue ;
			const auto& info2 = it2->second;
			auto it3 = bitmaps_.find(info2.path);
			if (it3 == bitmaps_.end()) continue ;

			Renderer::GetInstance()->DrawImage(it3->second.get(),
				static_cast<Anchor>(info.anchor),
				info.x,  info.y,  info.w,  info.h,
				info2.x, info2.y, info2.w, info2.h,
				Gdiplus::Color(info.a, info.r, info.g, info.b));
		} else if (info.type == 1) {
			// 文字列描画
			Renderer::GetInstance()->DrawString(
				static_cast<Anchor>(info.anchor), info.x, info.y, info.h, 
				Gdiplus::Color(info.a, info.r, info.g, info.b),
				info.str.c_str());
		}
	}

	// フェード
	Renderer::GetInstance()->FillRect(0, 0, GetWidth(), GetHeight(), Gdiplus::Color(fade_counter_, 0, 0, 0));

	// 操作説明
	int y = 0;
	Renderer::GetInstance()->DrawString("マスター読み込み:R",      0, y++ * 16, 12);
	Renderer::GetInstance()->DrawString("フェードイン:I",          0, y++ * 16, 12);
	Renderer::GetInstance()->DrawString("フェードアウト:O",        0, y++ * 16, 12);
	Renderer::GetInstance()->DrawString("とまる～を再度移動:S",    0, y++ * 16, 12);
	Renderer::GetInstance()->DrawString("Bezier::Linear:1",        0, y++ * 16, 12);
	Renderer::GetInstance()->DrawString("Bezier::EaseIn:2",        0, y++ * 16, 12);
	Renderer::GetInstance()->DrawString("Bezier::EaseOut:3",       0, y++ * 16, 12);
	Renderer::GetInstance()->DrawString("Bezier::EaseInOut:4",     0, y++ * 16, 12);
	Renderer::GetInstance()->DrawString("Bezier::EaseInBack:5",    0, y++ * 16, 12);
	Renderer::GetInstance()->DrawString("Bezier::EaseOutBack:6",   0, y++ * 16, 12);
	Renderer::GetInstance()->DrawString("Bezier::EaseInOutBack:7", 0, y++ * 16, 12);
	Renderer::GetInstance()->DrawString("Bezier::EaseInCirc:8",    0, y++ * 16, 12);

	// FPS表示
	Renderer::GetInstance()->DrawString(RIGHT_TOP, GetWidth(), 0, 16, Gdiplus::Color::White, _T("FPS:%.1f"), GetAverageFPS());
}

/*!
 * @brief マスター再読み込み
 */
void MainApp::Reload_()
{
	// マスターデータを再読み込み
	MasterData::Reload("data/master");

	// 画像を再読み込み
	bitmaps_.clear();
	std::set<std::string> images;
	for (auto it = MasterData::TitleImageList.begin(); it != MasterData::TitleImageList.end(); ++it) {
		images.insert(it->second.path);
	}
	for (auto it = images.begin(); it != images.end(); ++it) {
		std::string path = std::string("data/image/") + *it;
		auto bmp = new Gdiplus::Bitmap(Utility::SJIStoUTF16(path).c_str());
		if (bmp->GetLastStatus() == Gdiplus::Ok) {
			bitmaps_[*it].reset(bmp);
		} else {
			delete bmp;
		}
	}

	// 操作しやすいようにデータ化する
	objects_.clear();
	for (auto it = MasterData::TitleUI.begin(); it != MasterData::TitleUI.end(); ++it) {
		objects_[it->name] = &(*it);
	}

	cursour_y_ = objects_["TextCursor"]->y;
}

void MainApp::SetBezier_(const Bezier::ControlPoint& cp)
{
	bezier_counter_.Set(100, 400, 60, cp);
	bezier_timer_.Set(100, 400, 1.0f, cp);
}

