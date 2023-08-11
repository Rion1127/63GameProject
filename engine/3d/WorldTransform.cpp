
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

	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
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

	//�e�I�u�W�F�̉�]�̂�
	if (parentRotMat_) {
		matRot *= *parentRotMat_;
	}
	//�e�I�u�W�F�̍��W�̂�
	if (parentPosMat_) {
		matTrans *= *parentPosMat_;
	}

	//���[���h�s��̍���
	matWorld_.UnitMatrix();//�ό`�����Z�b�g
	//�r���{�[�h
	if (isBillboard == 1)
	{
		matWorld_ *= Camera::scurrent_->matBillboard_;
	}
	else if (isBillboard == 2)
	{
		matWorld_ *= Camera::scurrent_->matBillboardY_;
	}

	matWorld_ *= matScale;			//���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld_ *= matRot;				//���[���h�s��ɉ�]�𔽉f
	matWorld_ *= matTrans;			//���[���h�s��ɕ��s�ړ��𔽉f

	//�e�I�u�W�F�N�g�������
	if (parent_)
	{
		//�e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld_ *= parent_->matWorld_;
	}
	

	//�萔�o�b�t�@�փf�[�^�]��
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

