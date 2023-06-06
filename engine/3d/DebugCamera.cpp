#include "DebugCamera.h"
#include <DirectXMath.h>
#include <WinUser.h>

void DebugCamera::DebugCameraIni()
{
	mInput_ = MouseInput::GetInstance();

	viewProjection.SetEyePos(0, 0, -200);
	viewProjection.SetTarget(0, 0, 0);
	viewProjection.SetUpVec(0, 1, 0);

	frontVec = { 0, 0, 0 };
	sideVec = { 0,0,0 };

	frontdist = 50;
}

void DebugCamera::Update()
{
	if (DirectXInput::GetInstance()->PushKey(DIK_LCONTROL))
	{
		//マウスの情報の更新
		CameraMove();
		viewProjection.Update();
	}
}

void DebugCamera::CameraMove()
{
	Vector3 proviUpVec = { 0,1,0 };
	float speed = 0.5f;
	//カメラが注視点座標より奥にいるとき
	if (viewProjection.up.y <= 0) {
		speed *= -1;
	}

	//カメラの正面ベクトル
	frontVec = {
		viewProjection.target.x - viewProjection.eye.x,
		viewProjection.target.y - viewProjection.eye.y,
		viewProjection.target.z - viewProjection.eye.z
	};
	frontVec.normalize();

	sideVec = proviUpVec.cross(frontVec);
	//sideVec.normalize();

	upVec = sideVec.cross(frontVec);
	//upVec.normalize();

	//平行移動
	if (mInput_->IsMouseDown(MOUSE_WHEEL)) {
		//上下に動かしたとき
		if (mInput_->GetCursorMoveY() > 0) {
			cameraTrans -= upVec * speed;
			viewProjection.target.x -= upVec.x * speed;
			viewProjection.target.y -= upVec.y * speed;
			viewProjection.target.z -= upVec.z * speed;
		}
		else if (mInput_->GetCursorMoveY() < 0) {
			cameraTrans += upVec * speed;
			viewProjection.target.x += upVec.x * speed;
			viewProjection.target.y += upVec.y * speed;
			viewProjection.target.z += upVec.z * speed;
		}
		//マウスカーソルを左右に動かしたとき
		if (mInput_->GetCursorMoveX() > 0) {
			cameraTrans.x -= sideVec.x * speed;
			cameraTrans.z -= sideVec.z * speed;

			viewProjection.target.x -= sideVec.x * speed;
			viewProjection.target.z -= sideVec.z * speed;
		}
		else if (mInput_->GetCursorMoveX() < 0) {
			cameraTrans.x += sideVec.x * speed;
			cameraTrans.z += sideVec.z * speed;

			viewProjection.target.x += sideVec.x * speed;
			viewProjection.target.z += sideVec.z * speed;
		}

	}
	//拡大縮小
	else if (!mInput_->IsMouseDown(MOUSE_WHEEL)) {
		frontdist += -mInput_->IsMouseWheel() * (frontdist * 0.001f);
	}
	//球面座標移動
	if (mInput_->IsMouseDown(MOUSE_LEFT)) {
		//カメラが上を向いてるとき通常通りに座標を足す
		if (viewProjection.up.y >= 0) {
			moveDist += mInput_->GetCursorMove() * 0.005f;
		}
		//カメラが逆さまになった時X.Z座標を反転させる
		else if (viewProjection.up.y <= 0) {
			moveDist.x -= mInput_->GetCursorMoveX() * 0.005f;
			moveDist.y += mInput_->GetCursorMoveY() * 0.005f;
			moveDist.z -= mInput_->GetCursorMoveZ() * 0.005f;
		}
	}

	//カメラUP変換
	viewProjection.up = {
		0,
		cosf(moveDist.y),
		0
	};

	viewProjection.eye.x = -frontdist * sinf(moveDist.x) * cosf(moveDist.y) + cameraTrans.x;
	viewProjection.eye.y = frontdist * sinf(moveDist.y) + cameraTrans.y;
	viewProjection.eye.z = -frontdist * cosf(moveDist.x) * cosf(moveDist.y) + cameraTrans.z;
}

Camera* DebugCamera::GetViewProjection()
{
	return &viewProjection;
}
