#include "Object3d.h"

Object3d::Object3d()
{
	pos = { 0,0,0 };
	scale = { 1,1,1 };
	rot = {0,0,0};
}

Object3d::~Object3d()
{
	delete model_;
}

void Object3d::Init()
{
}

void Object3d::Update()
{
	WT_.position = pos;
	WT_.scale = scale;
	WT_.rotation = rot;
	WT_.Update();
}

void Object3d::Draw()
{
	model_->DrawOBJ(&WT_);
}
