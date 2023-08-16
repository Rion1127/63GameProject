#include <cassert>

#include "mInput.h"
#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "dinput8.lib")

//DirectInput�̏�����
static IDirectInput8* sdirectInput = nullptr;

IDirectInputDevice8* Key::skeyboard_ = nullptr;
//�̓��͏�Ԃ��擾����
BYTE Key::skeys_[256] = {};
//�̓��͏�Ԃ��擾����
BYTE Key::soldkeys_[256] = {};

#pragma region �L�[�{�[�h

void Key::InputIni()	//������
{
	HRESULT result;

	//DirectInput�̏�����
	result = DirectInput8Create(
		WinAPI::GetInstance()->w_.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&sdirectInput, nullptr);
	assert(SUCCEEDED(result));

	//�L�[�{�[�h�f�o�C�X�̐���
	result = sdirectInput->CreateDevice(GUID_SysKeyboard, &skeyboard_, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = skeyboard_->SetDataFormat(&c_dfDIKeyboard);	//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = skeyboard_->SetCooperativeLevel(
		WinAPI::GetInstance()->hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
	//�g���Ă���t���O�ɂ���
	//DISCL_FOREGROUND		��ʂ���O�ɂ���ꍇ�̂ݓ��͂��󂯕t����
	//DISCL_NONEXCLUSIVE	�f�o�C�X�����̃A�v�������Ő�L���Ȃ�
	//DISCL_NOWINKEY		Windows�L�[�𖳌������� 

	//�L�[�{�[�h���̎擾�J�n
	skeyboard_->Acquire();
}

void Key::InputUpdata()	//�A�b�v�f�[�g
{
	for (uint32_t i = 0; i < 256; ++i)
	{
		soldkeys_[i] = skeys_[i];
	}
	//�L�[�{�[�h���̎擾�J�n
	skeyboard_->Acquire();
	skeyboard_->GetDeviceState(sizeof(skeys_), skeys_);
}
//�������ςȂ�
bool Key::PushKey(UINT8 key)
{
	return skeys_[key];
}
//�������u��
bool Key::TriggerKey(UINT8 key)
{
	return skeys_[key] && !soldkeys_[key];
}
//�������u��
bool Key::GetKeyReleased(UINT8 key)
{
	return !skeys_[key] && soldkeys_[key];
}
#pragma endregion

#pragma region �}�E�X
#define MOUSE_INPUT 0x80

MouseInput* MouseInput::GetInstance()
{
	static MouseInput instance;
	return &instance;
}

void MouseInput::MouseIni()
{
	HRESULT result;

	//�L�[�{�[�h�f�o�C�X�̐���
	result = sdirectInput->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(result));
	//���̓f�[�^�`���̃Z�b�g
	result = mouse_->SetDataFormat(&c_dfDIMouse);	//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = mouse_->SetCooperativeLevel(
		WinAPI::GetInstance()->hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
	//�g���Ă���t���O�ɂ���
	//DISCL_FOREGROUND		��ʂ���O�ɂ���ꍇ�̂ݓ��͂��󂯕t����
	//DISCL_NONEXCLUSIVE	�f�o�C�X�����̃A�v�������Ő�L���Ȃ�
	//DISCL_NOWINKEY		Windows�L�[�𖳌������� 

	//�}�E�X���̎擾�J�n
	mouse_->Acquire();
}

void MouseInput::GetCursorPosition()
{
	//�X�N���[�����猩���}�E�X�̍��W���擾����
	GetCursorPos(&p_);
	//�E�B���h�E���猩���}�E�X�̍��W���擾����
	ScreenToClient(WinAPI::GetInstance()->hwnd_, &p_);
	//�O�t���[���̏�Ԃ�������
	prevmPos_ = mPos_;
	//���t���[���̍��W��������
	mPos_.x = (float)p_.x;
	mPos_.y = (float)p_.y;
	//�}�E�X���ǂ̕����ɓ��������̃x�N�g�����擾
	mouseVec_ = mPos_ - prevmPos_;

}
#include <string>
void MouseInput::Updata()
{
	HRESULT result;

	//�}�E�X���̎擾�J�n
	mouse_->Acquire();	//�����ɒu�������Ƃŉ���

	//std::string str = "OK\n";
	//�O�t���[���̏�Ԃ���
	prevmouseState_ = mouseState_;
	//�}�E�X���̎擾�J�n
	result = mouse_->Poll();
	if (result == DIERR_INPUTLOST) {
		//str = "NG\n";
	}

	result = mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState_);
	//�E�B���h�E�̊O���N���b�N��������͏��𖳌��ɂ���
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
	//���W�擾
	GetCursorPosition();
}
//���N���b�N���ꂽ��
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

#pragma region �R���g���[��
XINPUT_STATE Controller::state_;
XINPUT_STATE Controller::preState_;
bool Controller::isConnect_;

//�o�C�u���[�V����
XINPUT_VIBRATION Controller::vibration_;
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

WORD Controller::GetButtons(PAD button)
{
	if (state_.Gamepad.wButtons == (WORD)button) {
		return true;
	}

	return false;
}

WORD Controller::GetTriggerButtons(PAD button)
{
	if ((state_.Gamepad.wButtons == (WORD)button) &&
		(preState_.Gamepad.wButtons != (WORD)button))
	{
		return true;
	}

	return false;
}

WORD Controller::GetReleasButtons(PAD button)
{
	if ((state_.Gamepad.wButtons != (WORD)button) &&
		(preState_.Gamepad.wButtons == (WORD)button))
	{
		return true;
	}

	return false;
}

Vector2 Controller::GetLStick(int32_t deadZone)
{
	Vector2 stickPos;

	//���X�e�B�b�N
	stickPos.x = state_.Gamepad.sThumbLX;
	stickPos.y = state_.Gamepad.sThumbLY;
	//�f�b�h�]�[����ݒ�
	if ((state_.Gamepad.sThumbLX < (SHORT)deadZone &&
		state_.Gamepad.sThumbLX > -(SHORT)deadZone))
	{
		stickPos.x = 0;
	}

	if ((state_.Gamepad.sThumbLY < (SHORT)deadZone &&
		state_.Gamepad.sThumbLY > -(SHORT)deadZone))
	{
		stickPos.y = 0;
	}

	return stickPos;
}

Vector2 Controller::GetRStick(int32_t deadZone)
{
	Vector2 stickPos;
	//�E�X�e�B�b�N
		//return����ϐ��ɒl����
	stickPos.x = state_.Gamepad.sThumbRX;
	stickPos.y = state_.Gamepad.sThumbRY;
	if ((state_.Gamepad.sThumbRX < (SHORT)deadZone &&
		state_.Gamepad.sThumbRX > -(SHORT)deadZone))
	{
		stickPos.x = 0;
	}

	if ((state_.Gamepad.sThumbRY < (SHORT)deadZone &&
		state_.Gamepad.sThumbRY > -(SHORT)deadZone))
	{
		stickPos.y = 0;
	}
	//�f�b�h�]�[���ɓ���Ȃ�������l��Ԃ�
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