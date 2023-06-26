#include <cassert>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include "mInput.h"

//DirectInputの初期化
static IDirectInput8* sdirectInput = nullptr;

IDirectInputDevice8* Key::skeyboard_ = nullptr;
//の入力状態を取得する
BYTE Key::skeys_[256] = {};
//の入力状態を取得する
BYTE Key::soldkeys_[256] = {};

#pragma region キーボード

void Key::InputIni()	//初期化
{
	HRESULT result;

	//DirectInputの初期化
	result = DirectInput8Create(
		WinAPI::GetInstance()->w_.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&sdirectInput, nullptr);
	assert(SUCCEEDED(result));

	//キーボードデバイスの生成
	result = sdirectInput->CreateDevice(GUID_SysKeyboard, &skeyboard_, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = skeyboard_->SetDataFormat(&c_dfDIKeyboard);	//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = skeyboard_->SetCooperativeLevel(
		WinAPI::GetInstance()->hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
	//使っているフラグについて
	//DISCL_FOREGROUND		画面が手前にある場合のみ入力を受け付ける
	//DISCL_NONEXCLUSIVE	デバイスをこのアプリだけで占有しない
	//DISCL_NOWINKEY		Windowsキーを無効化する 
	
	//キーボード情報の取得開始
	skeyboard_->Acquire();
}

void Key::InputUpdata()	//アップデート
{
	for (uint32_t i = 0; i < 256; ++i)
	{
		soldkeys_[i] = skeys_[i];
	}
	//キーボード情報の取得開始
	skeyboard_->Acquire();
	skeyboard_->GetDeviceState(sizeof(skeys_), skeys_);
}
//押しっぱなし
bool Key::PushKey(UINT8 key)
{
	return skeys_[key];
}
//押した瞬間
bool Key::TriggerKey(UINT8 key)
{
	return skeys_[key] && !soldkeys_[key];
}
//離した瞬間
bool Key::GetKeyReleased(UINT8 key)
{
	return !skeys_[key] && soldkeys_[key];
}
#pragma endregion

#pragma region マウス
#define MOUSE_INPUT 0x80

MouseInput* MouseInput::GetInstance()
{
	static MouseInput instance;
	return &instance;
}

void MouseInput::MouseIni()
{
	HRESULT result;
	
	//キーボードデバイスの生成
	result = sdirectInput->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(result));
	//入力データ形式のセット
	result = mouse_->SetDataFormat(&c_dfDIMouse);	//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = mouse_->SetCooperativeLevel(
		WinAPI::GetInstance()->hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
	//使っているフラグについて
	//DISCL_FOREGROUND		画面が手前にある場合のみ入力を受け付ける
	//DISCL_NONEXCLUSIVE	デバイスをこのアプリだけで占有しない
	//DISCL_NOWINKEY		Windowsキーを無効化する 

	//マウス情報の取得開始
	mouse_->Acquire();
}

void MouseInput::GetCursorPosition()
{
	//スクリーンから見たマウスの座標を取得する
	GetCursorPos(&p_);
	//ウィンドウから見たマウスの座標を取得する
	ScreenToClient(WinAPI::GetInstance()->hwnd_, &p_);
	//前フレームの状態を代入する
	prevmPos_ = mPos_;
	//現フレームの座標を代入する
	mPos_.x = (float)p_.x;
	mPos_.y = (float)p_.y;
	//マウスがどの方向に動いたかのベクトルを取得
	mouseVec_ = mPos_ - prevmPos_;
	//ベクトル正規化
	//mouseVec.normalize();
}
#include <string>
void MouseInput::Updata()
{
	HRESULT result;
	
	//マウス情報の取得開始
	mouse_->Acquire();	//ここに置いたことで解決

	//std::string str = "OK\n";
	//前フレームの状態を代入
	prevmouseState_ = mouseState_;
	//マウス情報の取得開始
	result = mouse_->Poll();
	if (result == DIERR_INPUTLOST) {
		//str = "NG\n";
	}
	
	result = mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState_);
	//ウィンドウの外をクリックしたら入力情報を無効にする
	if (FAILED(result)) {
		mouseState_.lX = 0;
		mouseState_.lY = 0;
		mouseState_.lZ = 0;
		mouseState_.rgbButtons[0] = 0;
		mouseState_.rgbButtons[1] = 0;
		mouseState_.rgbButtons[2] = 0;
		mouseState_.rgbButtons[3] = 0;
	}
	//OutputDebugStringA(str.c_str());
	//座標取得
	GetCursorPosition();
}
//左クリックされたら
bool MouseInput::IsMouseTrigger(BYTE button)
{
	if (!prevmouseState_.rgbButtons[button] &&
		mouseState_.rgbButtons[button]) {
		return true;
	}
	return false;
}

bool MouseInput::IsMouseDown(BYTE button)
{
	if (prevmouseState_.rgbButtons[button]) {
		return true;
	}
	return false;
}

bool MouseInput::IsMouseReleas(BYTE button)
{
	if ((prevmouseState_.rgbButtons[button]) &&
		!(mouseState_.rgbButtons[button])) {
		return true;
	}
	return false;
}

int32_t MouseInput::IsMouseWheel()
{
	return mouseState_.lZ;
}

Vector3 MouseInput::GetCursorMove()
{
	return mouseVec_;
}

float MouseInput::GetCursorMoveX()
{
	return mouseVec_.x;
}

float MouseInput::GetCursorMoveY()
{
	return mouseVec_.y;
}

float MouseInput::GetCursorMoveZ()
{
	return mouseVec_.z;
}
#pragma endregion

#pragma region コントローラ
Controller* Controller::GetInstance()
{
	static Controller instance;
	return &instance;
}

void Controller::Ini()
{
	Update();
}

void Controller::Update()
{
	DWORD result = S_OK;
	preState_ = state_;
	result = XInputGetState(
		0,       // DWORD         dwUserIndex
		&state_);

	if (result == ERROR_SUCCESS) {
		isConnect_ = true;
	}
	else {
		isConnect_ = false;
	}
}

WORD Controller::GetButtons(WORD button)
{
	if (state_.Gamepad.wButtons == button) {
		return true;
	}

	return false;
}

WORD Controller::GetTriggerButtons(WORD button)
{
	if ((state_.Gamepad.wButtons == button) &&
		(preState_.Gamepad.wButtons != button))
	{
		return true;
	}

	return false;
}

WORD Controller::GetReleasButtons(WORD button)
{
	if ((state_.Gamepad.wButtons != button) &&
		(preState_.Gamepad.wButtons == button))
	{
		return true;
	}

	return false;
}

Vector2 Controller::GetLStick()
{
	Vector2 stickPos;
	
	//左スティック
	stickPos.x = state_.Gamepad.sThumbLX;
	stickPos.y = state_.Gamepad.sThumbLY;
	//デッドゾーンを設定
	if ((state_.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE/* * 2.0f*/ &&
		state_.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE/* * 2.0f*/))
	{
		stickPos.x = 0;
	}

	if ((state_.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE/* * 2.0f*/ &&
		state_.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE/* * 2.0f*/))
	{
		stickPos.y = 0;
	}

	return stickPos;
}

Vector2 Controller::GetRStick()
{
	Vector2 stickPos;
	//右スティック
		//returnする変数に値を代入
	stickPos.x = state_.Gamepad.sThumbRX;
	stickPos.y = state_.Gamepad.sThumbRY;
	//デッドゾーンを設定
	if ((state_.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		state_.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(state_.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			state_.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		return Vector2(0, 0);
	}
	//デッドゾーンに入らなかったら値を返す
	return stickPos;
}

BYTE Controller::GetRTrigger()
{
	return state_.Gamepad.bRightTrigger;
}

BYTE Controller::GetLTrigger()
{
	return state_.Gamepad.bLeftTrigger;
}

#pragma endregion