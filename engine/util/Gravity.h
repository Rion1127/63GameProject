#pragma once
#include "vector3.h"

/**
 * @file Gravity.h
 * @brief 重力の機能を持ったクラス
 */

class Gravity
{
private:
	Vector3 addValue_;
	Vector3 gravityVec_;
	Vector3 gravityLimit_;
public:
	Gravity();
	void Update();

public:
	void SetGrabity(const Vector3& gravity) { gravityVec_ = gravity; }
	void SetAddValue(const Vector3& value) { addValue_ = value; }
	void SetGravityLimit(const Vector3& limit) { gravityLimit_ = limit; }
public:
	Vector3 GetGravityValue() { return gravityVec_; }
	Vector3 GetAddValue() { return addValue_; }
};

