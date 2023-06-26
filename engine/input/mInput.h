#pragma once
#define DIRECTINPUT_VERSION		0x0800	//DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "Vector3.h"
#include <WinUser.h>
#include "WinAPI.h"
//�R���g���[��
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")

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
	//�}�E�X�̍��W���擾����
	void GetCursorPosition();
};
//�R���g���[��
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

	//�o�C�u���[�V����
	XINPUT_VIBRATION vibration_;

public:
	static Controller* GetInstance();

	void Ini();

	void Update();

	bool GetActive() { return isConnect_; }

	WORD GetButtons(WORD button);
	WORD GetTriggerButtons(WORD button);
	WORD GetReleasButtons(WORD button);
	//false	�E�X�e�B�b�N
	//true	���X�e�B�b�N
	Vector2 GetLStick();
	Vector2 GetRStick();

	BYTE GetRTrigger();
	BYTE GetLTrigger();
};

