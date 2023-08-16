#pragma once
#define DIRECTINPUT_VERSION		0x0800	//DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "Vector3.h"
#include <WinUser.h>
#include "WinAPI.h"
//�R���g���[��
#include <Xinput.h>


//�L�[�{�[�h
class Key
{
private:
	static IDirectInputDevice8* skeyboard_;
	//�S�L�[�̓��͏�Ԃ��擾����
	static BYTE skeys_[256];
	//�S�L�[�̓��͏�Ԃ��擾����
	static BYTE soldkeys_[256];
public:
	static void InputIni();
	static void InputUpdata();

	static bool PushKey(UINT8 key);		//�������ςȂ�
	static bool TriggerKey(UINT8 key);		//�������u��
	static bool GetKeyReleased(UINT8 key);
};
//�}�E�X
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
	//���t���[���̃}�E�X�̈ʒu
	Vector3 mPos_;
	//�O�t���[���̃}�E�X�̈ʒu
	Vector3 prevmPos_;
	//�}�E�X�������������̃x�N�g��
	Vector3 mouseVec_;
public:
	static MouseInput* GetInstance();

	void MouseIni();
	void Updata();

	//�}�E�X�{�^�����
	//[0] MOUSE_LEFT= ���{�^��
	//[1] MOUSE_RIGHT= �E�{�^��
	//[2] MOUSE_WHEEL= �}�E�X�z�C�[��
	bool IsMouseTrigger(BYTE button);
	//[0] MOUSE_LEFT= ���{�^��
	//[1] MOUSE_RIGHT= �E�{�^��
	//[2] MOUSE_WHEEL= �}�E�X�z�C�[��
	bool IsMouseDown(BYTE button);
	//[0] MOUSE_LEFT= ���{�^��
	//[1] MOUSE_RIGHT= �E�{�^��
	//[2] MOUSE_WHEEL= �}�E�X�z�C�[��
	bool IsMouseReleas(BYTE button);
	int32_t IsMouseWheel();
	
	//�}�E�X��1�t���[���Ɉړ������x�N�g�����擾����
	Vector3 GetCursorMove();

	float GetCursorMoveX();
	float GetCursorMoveY();
	float GetCursorMoveZ();
private:
	MouseInput() {};
	//�}�E�X�̍��W���擾����
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

//�R���g���[��
class Controller {
private:
	static XINPUT_STATE state_;
	static XINPUT_STATE preState_;
	static bool isConnect_;

	//�o�C�u���[�V����
	static XINPUT_VIBRATION vibration_;

public:
	
	static void Ini();

	static void Update();

	static bool GetActive() { return isConnect_; }

	static WORD GetButtons(PAD button);
	static WORD GetTriggerButtons(PAD button);
	static WORD GetReleasButtons(PAD button);
	//false	�E�X�e�B�b�N
	//true	���X�e�B�b�N
	static Vector2 GetLStick(int32_t deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	static Vector2 GetRStick(int32_t deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

	static BYTE GetRTrigger();
	static BYTE GetLTrigger();
private:
	Controller() {};
};

