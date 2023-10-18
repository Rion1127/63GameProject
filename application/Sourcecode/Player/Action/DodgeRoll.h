#pragma once
#include "Vector3.h"
#include "Timer.h"
class DodgeRoll
{
private:
	Vector3 dodgeVec_;
	float speed_;
	TimerFloat dodgetimer_;
	bool isDodge_;
public:
	DodgeRoll();
	void Begin(const Vector2& frontVec);
	void Update();
public:
	Vector3 GetDodgeVec() { return dodgeVec_; }
	bool GetIsDodge() { return isDodge_; }
	TimerFloat GetdodgeTimer() { return dodgetimer_; }
};

