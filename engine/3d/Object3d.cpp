#include "Object3d.h"

/**
 * @file Object3d.cpp
 * @brief 読み込んだモデルを描画するクラス
 */

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
	model_->ShadowUpdate(WT_.position_);
	WT_.Update();
}

void Object3d::Draw()
{
	if (isVisible_)
	{
		model_->DrawOBJ(WT_);
	}
}
