
#include "Util.h"
#include "Camera.h"
#include "WorldTransform.h"


WorldTransform::WorldTransform()
{
	position_ = { 0,0,0 };
	scale_ = { 1,1,1 };
	rotation_ = { 0,0,0 };

	constBuffTransform_ = CreateBuff(constMapTransform_);
}

void WorldTransform::SetScale(Vector3 scale)
{
	scale_ = scale;
}

void WorldTransform::SetRotation(Vector3 rot)
{
	rotation_ = rot;
}

void WorldTransform::SetPosition(Vector3 pos)
{
	position_ = pos;
}

void WorldTransform::AddScale(Vector3 scale)
{
	scale_ += scale;
}

void WorldTransform::AddRotation(Vector3 rot)
{
	rotation_ +=rot;
}

void WorldTransform::AddPosition(Vector3 pos)
{
	position_ += pos;
}



void WorldTransform::Update(uint32_t isBillboard)
{
	Matrix4 matScale, matRot, matTrans;

	//スケール、回転、平行移動行列の計算
	matScale = ConvertScalingMat(scale_);
	matRot.UnitMatrix();
	matRot *= ConvertRotationZAxisMat(rotation_.z);
	matRot *= ConvertRotationXAxisMat(rotation_.x);
	matRot *= ConvertRotationYAxisMat(rotation_.y);
	matTrans = ConvertTranslationMat(position_);

	
	//クォータニオン
	/*q.w += 0.005f;

	matRot = q.UpdateMatrix();*/

	//ワールド行列の合成
	matWorld_.UnitMatrix();//変形をリセット
	//ビルボード
	if (isBillboard == 1) {
		matWorld_ *= Camera::scurrent_.matBillboard_;
	}
	else if (isBillboard == 2) {
		matWorld_ *= Camera::scurrent_.matBillboardY_;
	}

	matWorld_ *= matScale;			//ワールド行列にスケーリングを反映
	matWorld_ *= matRot;				//ワールド行列に開店を反映
	matWorld_ *= matTrans;			//ワールド行列に平行移動を反映

	//親オブジェクトがあれば
	if (parent_) {
		//親オブジェクトのワールド行列を掛ける
		matWorld_ *= parent_->matWorld_;
	}

	//定数バッファへデータ転送
	constMapTransform_->mat = matWorld_;
	constMapTransform_->viewProj = Camera::scurrent_.GetMatView() * Camera::scurrent_.GetMatProjection();
	constMapTransform_->cameraPos = {
		Camera::scurrent_.eye_.x,
		Camera::scurrent_.eye_.y,
		Camera::scurrent_.eye_.z
	};
}

