#pragma once
#include "AssimpModel.h"
#include "WorldTransform.h"
class AssimpObject3D
{
public:
	void SetModel(AssimpModel* model) { model_ = model; }

	void Update();

	void Draw();

private:
	AssimpModel* model_ = nullptr;
	WorldTransform worldTransform_;

public:
	void SetPos(Vector3 pos) { worldTransform_.position_ = pos; }
	void SetScale(Vector3 scale) { worldTransform_.scale_ = scale; }
	void SetRot(Vector3 rot) { worldTransform_.rotation_ = rot; }
};

