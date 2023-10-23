#include "Gravity.h"
#include "Util.h"

/**
 * @file Gravity.cpp
 * @brief 重力の機能を持ったクラス
 */

Gravity::Gravity()
{
	addValue_ = { 0, -0.015f,0 };
	gravityVec_ = { 0,0,0 };
	gravityLimit_ = { 0,-0.5f,0 };
	isActive_ = true;
}

void Gravity::Update()
{
	if (isActive_) {
		gravityVec_ += addValue_;
		gravityVec_ = {
			Max(gravityVec_.x,gravityLimit_.x),
			Max(gravityVec_.y,gravityLimit_.y),
			Max(gravityVec_.z,gravityLimit_.z),
		};
	}
	else {
		gravityVec_ = { 0,0,0 };
	}
}
