#pragma once
#include "vector3.h"
class Gravity
{
private:
	Vector3 addValue_;
	Vector3 gravityVec_;
public:
	Gravity();
	void Update();

public:
	void SetGrabity(const Vector3 &gravity) { gravityVec_ = gravity; }
	void SetAddValue(Vector3 value) { addValue_ = value; }
public:
	Vector3 GetGravityValue() { return gravityVec_; }
	Vector3 GetAddValue() { return addValue_; }
};

