#include "Object3d.h"

Object3d::Object3d()
{
}

Object3d::~Object3d()
{
}

void Object3d::Init()
{
}

void Object3d::Update()
{
	WT_.Update();
}

void Object3d::Draw()
{
	model_->DrawOBJ(WT_);
}
