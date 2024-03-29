#include "WinAPI.h"
#include <imgui_impl_win32.h>
#pragma comment(lib, "winmm.lib")

/**
 * @file WinAPI.cpp
 * @brief WinAPIについてまとめたクラス
 */

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

// ウィンドウプロシージャ
LRESULT WinAPI::WindowProcA(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//ImGui用ウィンドウプロシージャ呼び出し
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}
	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WinAPI* WinAPI::GetInstance()
{
	static WinAPI instance;
	return &instance;
}

void WinAPI::Ini()
{
	w_.cbSize = sizeof(WNDCLASSEX);
	w_.lpfnWndProc = (WNDPROC)WindowProcA;		// ウィンドウプロシージャを設定
	w_.lpszClassName = L"KING HEART";			// ウィンドウクラス名
	w_.hInstance = GetModuleHandle(nullptr);		// ウィンドウハンドル
	w_.hCursor = LoadCursor(NULL, IDC_ARROW);	// カーソル指定
	// ウィンドウクラスをOSに登録する
	RegisterClassEx(&w_);

	// 自動でサイズを補正する
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);

	hwnd_ = CreateWindow(w_.lpszClassName,	// クラス名
		L"KING HEART",						// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,				// 標準的なウィンドウスタイル
		CW_USEDEFAULT,						// 表示X座標(OSに任せる)
		CW_USEDEFAULT,						// 表示Y座標(OSに任せる)
		wrc_.right - wrc_.left,				// ウィンドウ横幅
		wrc_.bottom - wrc_.top,				// ウィンドウ縦幅
		nullptr,							// 親ウィンドウハンドル
		nullptr,							// メニューハンドル
		w_.hInstance,						// 呼び出しアプリケーションハンドル
		nullptr);							// オプション
	// ウィンドウを表示状態にする
	ShowWindow(hwnd_, SW_SHOW);

	//システムタイマーの分解能を上げる
	timeBeginPeriod(1);
}

bool WinAPI::MsgCheck()
{
	MSG msg{}; // メッセージ

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // メッセージがある？
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg);  // ウィンドウプロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT) // 終了メッセージが来たらループを抜ける
	{
		return true;
	}

	return false;
}

void WinAPI::ReleaseClass()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(w_.lpszClassName, w_.hInstance);
}




