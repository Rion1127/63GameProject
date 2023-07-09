#pragma once
#include "AssimpModel.h"
#include "WorldTransform.h"
class AssimpObject3D
{
public:

	void Update();

	void Draw();

private:
	std::unique_ptr<AssimpModel> model_;
	WorldTransform worldTransform_;

public:
	void SetPos(Vector3 pos) { worldTransform_.position_ = pos; }
	void SetScale(Vector3 scale) { worldTransform_.scale_ = scale; }
	void SetRot(Vector3 rot) { worldTransform_.rotation_ = rot; }
};

