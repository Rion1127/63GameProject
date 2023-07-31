#pragma once
#include "Vector3.h"
#include "Timer.h"
class DodgeRoll
{
private:
	Vector3 dodgeVec_;
	float speed_;
	Timer dodgetimer_;
public:
	void Begin(Vector2 frontVec);
	void Update();
public:
	Vector3 GetDodgeVec() { return dodgeVec_; }
};

