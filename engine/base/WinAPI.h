#pragma once
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
class WinAPI
{
public:

	static WinAPI* GetInstance();

	void Ini();

	bool MsgCheck();

	void ReleaseClass();
	
	static LRESULT WindowProcA(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	// �E�B���h�E�T�C�Y
	const int win_width = 1280; // ����
	const int win_height = 720; // �c��

	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};

	// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	RECT wrc = { 0, 0, win_width, win_height };

	HWND hwnd;
private:

};


