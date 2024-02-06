#include "DodgeRoll.h"
#include "Util.h"

/**
 * @file DodgeRoll.cpp
 * @brief プレイヤーの回避の処理をしている
 */

DodgeRoll::DodgeRoll()
{
	speed_ = 0.4f;
	dodgetimer_.SetLimitTime(30);
	speedAttenValue_ = 0.015f;
}

void DodgeRoll::Begin(const Vector2& frontVec)
{
	dodgetimer_.Reset();
	dodgeVec_ = {
		frontVec.x * speed_,
		0,
		frontVec.y * speed_,
	};
	isDodge_ = true;
}

void DodgeRoll::Update()
{
	dodgetimer_.AddTime();
	MoveTo({ 0,0,0 }, speedAttenValue_, dodgeVec_);

	if (dodgetimer_.GetIsEnd()) {
		dodgetimer_.Reset();
		isDodge_ = false;
	}
}
