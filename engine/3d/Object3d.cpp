#include "Object3d.h"

/**
 * @file Object3d.cpp
 * @brief 読み込んだモデルを描画するクラス
 */

Object3d::Object3d()
{
	billBoard = BillBoard::None;
}

Object3d::~Object3d()
{
}

void Object3d::Init()
{
}

void Object3d::Update(Camera* camera)
{
	if (model_ != nullptr) {
		model_->ShadowUpdate(WT_.position_);
	}
	WT_.Update(billBoard, camera);
}

void Object3d::Draw()
{
	if (isVisible_)
	{
		model_->DrawOBJ(WT_);
	}
}

void Object3d::Draw(WorldTransform WT)
{
	if (isVisible_)
	{
		model_->DrawOBJ(WT);
	}
}
