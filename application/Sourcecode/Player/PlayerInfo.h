#pragma once
#include "WorldTransform.h"
#include "Vector3.h"
#include "Gravity.h"

/**
 * @file PlayerInfo.h
 * @brief プレイヤーのステートを宣言をしている
 */

enum class PlayerState {
	Idle,
	Move,
	Jump,
	Attack,
	AirAttack,
	Freeze,
	Knock,
	Guard,
	DodgeRoll,
};
