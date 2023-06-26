#pragma once
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include <stdint.h>
#include "Vector2.h"
class WinAPI
{
public:

	static WinAPI* GetInstance();

	void Ini();

	bool MsgCheck();

	void ReleaseClass();
	
	static LRESULT WindowProcA(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	
	static Vector2 GetWindowSize() { return { win_width_ ,win_height_ }; };

	// ウィンドウクラスの設定
	WNDCLASSEX w_{};

	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc_ = { 0, 0, win_width_, win_height_ };

	HWND hwnd_;
private:
	// ウィンドウサイズ
	static const uint32_t win_width_ = 1280; // 横幅
	static const uint32_t win_height_ = 720; // 縦幅
};


