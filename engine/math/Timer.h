#pragma once
class Timer
{
public:
	//�^�C�}�[���Z�b�g
	void TimeReset();
	//�v���J�n
	void TimerStart();
	//�X�V
	void Update();
	//�v���I��
	void TimerStop();
	//���݂̃^�C����Ԃ�
	int GetTime();

private:
	int time;

	int baseTime;
	int nowTime;

	bool isActive;
};

