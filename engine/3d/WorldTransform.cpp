
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

	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = ConvertScalingMat(scale_);
	matRot.UnitMatrix();
	matRot *= ConvertRotationZAxisMat(rotation_.z);
	matRot *= ConvertRotationXAxisMat(rotation_.x);
	matRot *= ConvertRotationYAxisMat(rotation_.y);
	matTrans = ConvertTranslationMat(position_);

	
	//�N�H�[�^�j�I��
	/*q.w += 0.005f;

	matRot = q.UpdateMatrix();*/

	//���[���h�s��̍���
	matWorld_.UnitMatrix();//�ό`�����Z�b�g
	//�r���{�[�h
	if (isBillboard == 1) {
		matWorld_ *= Camera::scurrent_.matBillboard_;
	}
	else if (isBillboard == 2) {
		matWorld_ *= Camera::scurrent_.matBillboardY_;
	}

	matWorld_ *= matScale;			//���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld_ *= matRot;				//���[���h�s��ɊJ�X�𔽉f
	matWorld_ *= matTrans;			//���[���h�s��ɕ��s�ړ��𔽉f

	//�e�I�u�W�F�N�g�������
	if (parent_) {
		//�e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld_ *= parent_->matWorld_;
	}

	//�萔�o�b�t�@�փf�[�^�]��
	constMapTransform_->mat = matWorld_;
	constMapTransform_->viewProj = Camera::scurrent_.GetMatView() * Camera::scurrent_.GetMatProjection();
	constMapTransform_->cameraPos = {
		Camera::scurrent_.eye_.x,
		Camera::scurrent_.eye_.y,
		Camera::scurrent_.eye_.z
	};
}

