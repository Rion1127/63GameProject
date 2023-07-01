#pragma once
#include "WorldTransform.h"
#include "Vector3.h"

enum class PlayerState {
	Idle,
	Jump,
	Attack,
	AirAttack
};

struct PlayerInfo
{
	WorldTransform* WT;
	Vector3* frontVec;
	Vector3* addVec_;
	float* gravity;
	PlayerState* state;
};

