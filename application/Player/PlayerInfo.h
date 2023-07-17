#pragma once
#include "WorldTransform.h"
#include "Vector3.h"
#include "Gravity.h"

enum class PlayerState {
	Idle,
	Jump,
	Attack,
	AirAttack,
	Landing,
	Knock
};
