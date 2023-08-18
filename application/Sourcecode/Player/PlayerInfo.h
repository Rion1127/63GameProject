#pragma once
#include "WorldTransform.h"
#include "Vector3.h"
#include "Gravity.h"

enum class PlayerState {
	Idle,
	Walk,
	Run,
	Jump,
	Attack,
	AirAttack,
	Freeze,
	Knock,
	Guard,
	DodgeRoll
};
