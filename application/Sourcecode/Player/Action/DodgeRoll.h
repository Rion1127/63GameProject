#pragma once
#include "Vector3.h"
#include "Timer.h"

/**
 * @file DodgeRoll.h
 * @brief プレイヤーの回避の処理をしている
 */

class DodgeRoll
{
private:
	Vector3 dodgeVec_;
	float speed_;
	TimerFloat dodgetimer_;
	bool isDodge_;
	float speedAttenValue_;
public:
	DodgeRoll();
	void Begin(const Vector2& frontVec);
	void Update();
public:
	Vector3 GetDodgeVec() { return dodgeVec_; }
	bool GetIsDodge() { return isDodge_; }
	TimerFloat GetdodgeTimer() { return dodgetimer_; }
};

