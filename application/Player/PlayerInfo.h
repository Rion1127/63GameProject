#pragma once
#include "WorldTransform.h"
#include "Vector3.h"
#include "Gravity.h"

enum class PlayerState {
	Idle,
	Jump,
	Attack,
	AirAttack,
	Landing
};
//外部のファイル(IAttack,Enemy)などでプレイヤーの情報に
//アクセスできるようにする
struct PlayerInfo
{
	WorldTransform* WT;
	Vector3* frontVec;
	Vector3* addVec_;
	Vector3* rot_;
	Gravity* gravity;
	PlayerState* state;
};

