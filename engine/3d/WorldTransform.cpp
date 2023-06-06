
#include "Util.h"
#include "Camera.h"
#include "WorldTransform.h"


WorldTransform::WorldTransform()
{
	position = { 0,0,0 };
	scale = { 1,1,1 };
	rotation = { 0,0,0 };

	constBuffTransform = CreateBuff(constMapTransform);
}

void WorldTransform::SetScale(float x, float y, float z)
{
	scale = { x,y,z };
}

void WorldTransform::SetRotation(float x, float y, float z)
{
	rotation = { x,y,z };
}

void WorldTransform::SetPosition(float x, float y, float z)
{
	position = { x,y,z };
}

void WorldTransform::AddScale(float x, float y, float z)
{
	scale.x += x;
	scale.y += y;
	scale.z += z;
}

void WorldTransform::AddRotation(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
}

void WorldTransform::AddPosition(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
}



void WorldTransform::Update(int isBillboard)
{
	XMMATRIX matScale, matRot, matTrans;

	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matTrans = XMMatrixTranslation(
		position.x, position.y, position.z);

	
	//�N�H�[�^�j�I��
	/*q.w += 0.005f;

	matRot = q.UpdateMatrix();*/

	//���[���h�s��̍���
	matWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	//�r���{�[�h
	if (isBillboard == 1) {
		matWorld *= Camera::current.matBillboard;
	}
	else if (isBillboard == 2) {
		matWorld *= Camera::current.matBillboardY;
	}

	matWorld *= matScale;			//���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot;				//���[���h�s��ɊJ�X�𔽉f
	matWorld *= matTrans;			//���[���h�s��ɕ��s�ړ��𔽉f

	//�e�I�u�W�F�N�g�������
	if (parent) {
		//�e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld *= parent->matWorld;
	}

	//�萔�o�b�t�@�փf�[�^�]��
	constMapTransform->mat = matWorld ;
	constMapTransform->viewProj = Camera::current.GetMatView() * Camera::current.GetMatProjection();
	constMapTransform->cameraPos = {
		Camera::current.eye.x,
		Camera::current.eye.y,
		Camera::current.eye.z
	};
}

