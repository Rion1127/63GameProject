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
class Key
{
private:
	static IDirectInputDevice8* skeyboard_;
	//全キーの入力状態を取得する
	static BYTE skeys_[256];
	//全キーの入力状態を取得する
	static BYTE soldkeys_[256];
public:
	static void InputIni();
	static void InputUpdata();

	static bool PushKey(UINT8 key);		//押しっぱなし
	static bool TriggerKey(UINT8 key);		//押した瞬間
	static bool GetKeyReleased(UINT8 key);
};
//マウス
enum {
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_WHEEL
};

class MouseInput {
private:
	IDirectInputDevice8* mouse_ = nullptr;

	DIMOUSESTATE mouseState_;
	DIMOUSESTATE prevmouseState_;
public:
	POINT p_;
	//現フレームのマウスの位置
	Vector3 mPos_;
	//前フレームのマウスの位置
	Vector3 prevmPos_;
	//マウスが動いた方向のベクトル
	Vector3 mouseVec_;
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
	int32_t IsMouseWheel();
	
	//マウスが1フレームに移動したベクトルを取得する
	Vector3 GetCursorMove();

	float GetCursorMoveX();
	float GetCursorMoveY();
	float GetCursorMoveZ();
private:
	//マウスの座標を取得する
	void GetCursorPosition();
};
//コントローラ
class Controller {
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

	XINPUT_STATE state_;
	XINPUT_STATE preState_;
	bool isConnect_;

	//バイブレーション
	XINPUT_VIBRATION vibration_;

public:
	static Controller* GetInstance();

	void Ini();

	void Update();

	bool GetActive() { return isConnect_; }

	WORD GetButtons(WORD button);
	WORD GetTriggerButtons(WORD button);
	WORD GetReleasButtons(WORD button);
	//false	右スティック
	//true	左スティック
	Vector2 GetLStick();
	Vector2 GetRStick();

	BYTE GetRTrigger();
	BYTE GetLTrigger();
};

