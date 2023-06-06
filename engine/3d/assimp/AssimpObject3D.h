#pragma once
#include "AssinpModel.h"
#include "WorldTransform.h"
class AssimpObject3D
{
public:
	void SetModel(AssinpModel* model) { model_ = model; }

	void Update();

	void Draw();

private:
	AssinpModel* model_ = nullptr;
	WorldTransform worldTransform_;

public:
	void SetPos(Vector3 pos) { worldTransform_.position = pos; }
	void SetScale(Vector3 scale) { worldTransform_.scale = scale; }
	void SetRot(Vector3 rot) { worldTransform_.rotation = rot; }
};

