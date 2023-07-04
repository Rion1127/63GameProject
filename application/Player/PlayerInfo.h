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
//�O���̃t�@�C��(IAttack,Enemy)�ȂǂŃv���C���[�̏���
//�A�N�Z�X�ł���悤�ɂ���
struct PlayerInfo
{
	WorldTransform* WT;
	Vector3* frontVec;
	Vector3* addVec_;
	Vector3* rot_;
	Gravity* gravity;
	PlayerState* state;
};

