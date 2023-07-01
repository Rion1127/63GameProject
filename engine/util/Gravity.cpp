#include "Gravity.h"

Gravity::Gravity()
{
	addValue_ = { 0, -0.015f,0 };
	gravityVec_ = { 0,0,0 };
}

void Gravity::Update()
{

	gravityVec_ += addValue_;
}
