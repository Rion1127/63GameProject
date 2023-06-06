#pragma once
#define DIRECTINPUT_VERSION		0x0800	//DirectInputのバージョン指定
#include <dinput.h>
#include "Vector3.h"
#include <WinUser.h>
#include "WinAPI.h"
//コントローラ
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")

//キーボード
class DirectXInput
{
public:
	static DirectXInput* GetInstance();

	void InputIni();
	void InputUpdata();

	bool PushKey(UINT8 key);		//押しっぱなし
	bool TriggerKey(UINT8 key);		//押した瞬間
	bool GetKeyReleased(UINT8 key);

private:
	IDirectInputDevice8* keyboard = nullptr;
	//全キーの入力状態を取得する
	BYTE keys[256] = {};
	//全キーの入力状態を取得する
	BYTE oldkeys[256] = {};

	WinAPI* winapi_ = nullptr;
};
//マウス
class MouseInput {
public:
	static MouseInput* GetInstance();

	void MouseIni();
	void Updata();

	//マウスボタン情報
	//[0] MOUSE_LEFT= 左ボタン
	//[1] MOUSE_RIGHT= 右ボタン
	//[2] MOUSE_WHEEL= マウスホイール
	bool IsMouseTrigger(BYTE button);
	//[0] MOUSE_LEFT= 左ボタン
	//[1] MOUSE_RIGHT= 右ボタン
	//[2] MOUSE_WHEEL= マウスホイール
	bool IsMouseDown(BYTE button);
	//[0] MOUSE_LEFT= 左ボタン
	//[1] MOUSE_RIGHT= 右ボタン
	//[2] MOUSE_WHEEL= マウスホイール
	bool IsMouseReleas(BYTE button);
	int IsMouseWheel();
	
	//マウスが1フレームに移動したベクトルを取得する
	Vector3 GetCursorMove();

	float GetCursorMoveX();
	float GetCursorMoveY();
	float GetCursorMoveZ();
	
	POINT p;
	//現フレームのマウスの位置
	Vector3 mPos;
	//前フレームのマウスの位置
	Vector3 prevmPos;
	//マウスが動いた方向のベクトル
	Vector3 mouseVec;
private:
	//マウスの座標を取得する
	void GetCursorPosition();

	IDirectInputDevice8* mouse = nullptr;

	DIMOUSESTATE mouseState;
	DIMOUSESTATE prevmouseState;
	//HWND* hwnd_ = nullptr;
	WinAPI* winapi_ = nullptr;
};
enum {
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_WHEEL
};
//コントローラ
class Controller {
public:
	static Controller* GetInstance();

	void Ini();

	void Update();

	bool GetActive() { return isConnect; }

	WORD GetButtons(WORD button);
	WORD GetTriggerButtons(WORD button);
	WORD GetReleasButtons(WORD button);
	//false	右スティック
	//true	左スティック
	Vector2 GetLStick();
	Vector2 GetRStick();

	BYTE GetRTrigger();
	BYTE GetLTrigger();
private:
#define PAD_UP				0x0001
#define PAD_DOWN			0x0002
#define PAD_LEFT			0x0004
#define PAD_RIGHT			0x0008
#define PAD_START			0x0010
#define PAD_BACK			0x0020
#define PAD_LEFT_THUMB		0x0040
#define PAD_RIGHT_THUMB		0x0080
#define PAD_LEFT_SHOULDER	0x0100
#define PAD_RIGHT_SHOULDER	0x0200
#define PAD_A				0x1000
#define PAD_B				0x2000
#define PAD_X				0x4000
#define PAD_Y				0x8000

	XINPUT_STATE state;
	XINPUT_STATE preState;
	bool isConnect;

	//バイブレーション
	XINPUT_VIBRATION vibration;

};

