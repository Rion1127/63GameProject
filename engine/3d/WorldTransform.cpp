
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

	//スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matTrans = XMMatrixTranslation(
		position.x, position.y, position.z);

	
	//クォータニオン
	/*q.w += 0.005f;

	matRot = q.UpdateMatrix();*/

	//ワールド行列の合成
	matWorld = XMMatrixIdentity();	//変形をリセット
	//ビルボード
	if (isBillboard == 1) {
		matWorld *= Camera::current.matBillboard;
	}
	else if (isBillboard == 2) {
		matWorld *= Camera::current.matBillboardY;
	}

	matWorld *= matScale;			//ワールド行列にスケーリングを反映
	matWorld *= matRot;				//ワールド行列に開店を反映
	matWorld *= matTrans;			//ワールド行列に平行移動を反映

	//親オブジェクトがあれば
	if (parent) {
		//親オブジェクトのワールド行列を掛ける
		matWorld *= parent->matWorld;
	}

	//定数バッファへデータ転送
	constMapTransform->mat = matWorld ;
	constMapTransform->viewProj = Camera::current.GetMatView() * Camera::current.GetMatProjection();
	constMapTransform->cameraPos = {
		Camera::current.eye.x,
		Camera::current.eye.y,
		Camera::current.eye.z
	};
}

