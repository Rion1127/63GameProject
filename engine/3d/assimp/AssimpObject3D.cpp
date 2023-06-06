#include "AssimpObject3D.h"

void AssimpObject3D::Update()
{
	worldTransform_.Update();
}

void AssimpObject3D::Draw()
{
	model_->Draw(worldTransform_);
}
