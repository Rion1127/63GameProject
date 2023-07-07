#pragma once
#define DIRECTINPUT_VERSION		0x0800	//DirectInputのバージョン指定
#include <dinput.h>
#include "Vector3.h"
#include <WinUser.h>
#include "WinAPI.h"
//コントローラ
#include <Xinput.h>


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
	MouseInput() {};
	//マウスの座標を取得する
	void GetCursorPosition();
};

enum class PAD{
	INPUT_UP				= 0x0001,
	INPUT_DOWN				= 0x0002,
	INPUT_LEFT				= 0x0004,
	INPUT_RIGHT				= 0x0008,
	INPUT_START				= 0x0010,
	INPUT_BACK				= 0x0020,
	INPUT_LEFT_THUMB		= 0x0040,
	INPUT_RIGHT_THUMB		= 0x0080,
	INPUT_LEFT_SHOULDER		= 0x0100,
	INPUT_RIGHT_SHOULDER	= 0x0200,
	INPUT_A					= 0x1000,
	INPUT_B					= 0x2000,
	INPUT_X					= 0x4000,
	INPUT_Y					= 0x8000,
};

enum DeadZone {
	Half = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / 2,
	Default = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE,
	Mul_2 = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE * 2,
	Mul_3 = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE * 3,
};

//コントローラ
class Controller {
private:
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

	WORD GetButtons(PAD button);
	WORD GetTriggerButtons(PAD button);
	WORD GetReleasButtons(PAD button);
	//false	右スティック
	//true	左スティック
	Vector2 GetLStick(int32_t deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	Vector2 GetRStick(int32_t deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

	BYTE GetRTrigger();
	BYTE GetLTrigger();
private:
	Controller() {};
};

