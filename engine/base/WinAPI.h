#pragma once
#include <d3dcompiler.h>
#include <d3d12.h>
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

	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w_{};

	// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	RECT wrc_ = { 0, 0, win_width_, win_height_ };

	HWND hwnd_;
private:
	// �E�B���h�E�T�C�Y
	static const uint32_t win_width_ = 1280; // ����
	static const uint32_t win_height_ = 720; // �c��
private:
	WinAPI(){};
};


