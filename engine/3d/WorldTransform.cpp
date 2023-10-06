
#include "Util.h"
#include "Camera.h"
#include "WorldTransform.h"


WorldTransform::WorldTransform()
{
	position_ = { 0,0,0 };
	scale_ = { 1,1,1 };
	rotation_ = { 0,0,0 };

	constBuffTransform_ = CreateBuff(constMapTransform_);

	rotType = RotType::Euler;
}

void WorldTransform::AddScale(float x, float y, float z)
{
	scale_.x += x;
	scale_.y += y;
	scale_.z += z;
}

void WorldTransform::AddRotation(float x, float y, float z)
{
	rotation_.x += x;
	rotation_.y += y;
	rotation_.z += z;
}

void WorldTransform::AddPosition(float x, float y, float z)
{
	position_.x += x;
	position_.y += y;
	position_.z += z;
}

void WorldTransform::Update(uint32_t isBillboard)
{
	Matrix4 matScale, matRot, matTrans;

	//スケール、回転、平行移動行列の計算
	matScale = ConvertScalingMat(scale_);
	matRot.UnitMatrix();
	if (rotType == RotType::Euler)
	{
		matRot *= ConvertRotationZAxisMat(rotation_.z);
		matRot *= ConvertRotationXAxisMat(rotation_.x);
		matRot *= ConvertRotationYAxisMat(rotation_.y);
	}
	else
	{
		matRot = ConvertRotationMat(quaternion_);
	}
	matTrans = ConvertTranslationMat(position_);

	scaleMat_ = matScale;
	rotMat_ = matRot;
	posMat_ = matTrans;

	//親オブジェの回転のみ
	if (parentRotMat_) {
		matRot *= *parentRotMat_;
	}
	//親オブジェの座標のみ
	if (parentPosMat_) {
		matTrans *= *parentPosMat_;
	}

	//ワールド行列の合成
	matWorld_.UnitMatrix();//変形をリセット
	//ビルボード
	if (isBillboard == 1)
	{
		matWorld_ *= Camera::scurrent_->matBillboard_;
	}
	else if (isBillboard == 2)
	{
		matWorld_ *= Camera::scurrent_->matBillboardY_;
	}

	matWorld_ *= matScale;			//ワールド行列にスケーリングを反映
	matWorld_ *= matRot;				//ワールド行列に回転を反映
	matWorld_ *= matTrans;			//ワールド行列に平行移動を反映

	//親オブジェクトがあれば
	if (parent_)
	{
		//親オブジェクトのワールド行列を掛ける
		matWorld_ *= parent_->matWorld_;
	}


	//定数バッファへデータ転送
	if (Camera::scurrent_ != nullptr)
	{
		constMapTransform_->mat = matWorld_;
		constMapTransform_->viewProj = Camera::scurrent_->GetMatView() * Camera::scurrent_->GetMatProjection();
		constMapTransform_->cameraPos = {
			Camera::scurrent_->eye_.x,
			Camera::scurrent_->eye_.y,
			Camera::scurrent_->eye_.z
		};
	}
}

