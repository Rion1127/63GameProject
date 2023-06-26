#include "Object3d.h"

Object3d::Object3d()
{
	pos_ = { 0,0,0 };
	scale_ = { 1,1,1 };
	rot_ = {0,0,0};

}

Object3d::~Object3d()
{
}

void Object3d::Init()
{
}

void Object3d::Update()
{
	WT_.position_ = pos_;
	WT_.scale_ = scale_;
	WT_.rotation_ = rot_;
	WT_.Update();
}

void Object3d::Draw()
{
	model_->DrawOBJ(WT_);
}
