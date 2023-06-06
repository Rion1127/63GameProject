#include <d3dx12.h>
#include <random>
#include "Util.h"
#include "Camera.h"

Camera Camera::current{};

Camera::Camera()
{

	directX_ = DirectXCommon::GetInstance();

	eye = { 0,0,-10 };
	target = { 0,0,0 };
	up = { 0,1,0 };

	Update();
}

void Camera::SetEyePos(float x, float y, float z)
{
	eye.x = x;
	eye.y = y;
	eye.z = z;
}

void Camera::SetEyePos(Vector3 pos)
{
	if (isShake) {
		originalPos.x = pos.x;
		originalPos.y = pos.y;
		originalPos.z = pos.z;
	}
	else if (isShake == false) {
		eye.x = pos.x;
		eye.y = pos.y;
		eye.z = pos.z;
	}
}

void Camera::SetTarget(float x, float y, float z)
{
	target.x = x;
	target.y = y;
	target.z = z;
}

void Camera::SetTarget(Vector3 pos)
{
	target.x = pos.x;
	target.y = pos.y;
	target.z = pos.z;
}

void Camera::SetUpVec(float x, float y, float z)
{
	up.x = x;
	up.y = y;
	up.z = z;
}

void Camera::SetUpVec(Vector3 upVec)
{
	up.x = upVec.x;
	up.y = upVec.y;
	up.z = upVec.z;
}

void Camera::MoveTo(Vector3 goal, float speed)
{
	Vector3 dir = goal - eye;
	float dirLength = dir.length2();
	if (dirLength < speed * speed)
	{
		eye.x = goal.x;
		eye.y = goal.y;
		eye.z = goal.z;
		return;
	}
	eye.x = eye.x + dir.SetLength(speed).x;
	eye.y = eye.y + dir.SetLength(speed).y;
	eye.z = eye.z + dir.SetLength(speed).z;
}


void Camera::Update()
{
	/*matView =
		XMMatrixLookAtLH(
			XMLoadFloat3(&eye),
			XMLoadFloat3(&target),
			XMLoadFloat3(&up));*/

#pragma region ビュー行列
			//視点座標
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	//注視点座標
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	//（仮の）上方向
	XMVECTOR upVector = XMLoadFloat3(&up);

	//カメラZ軸（視線方向）
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	//0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	//ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);
	//カメラのX軸（右方向）
	XMVECTOR cameraAxisX;
	//X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	//ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);
	//カメラのY座標（上方向）
	XMVECTOR cameraAxisY;
	//Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	//cameraAxisY = XMVector3Normalize(cameraAxisY);
	//カメラ回転行列
	XMMATRIX matCameraRot{};
	//カメラ座標系→ワールド座標系の返還行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	//転置により逆用列（逆回転）を計算
	matView = XMMatrixTranspose(matCameraRot);
	//視点座標に-1を賭けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	//カメラの位置からワールド原点へのベクトル（カメラ座標系）
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);
	//一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	//ビュー行列に平行移動成分を設定
	matView.r[3] = translation;

	//全方向ビルボード行列
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

	//Y軸周りビルボード行列
	//カメラXYZ軸
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;
	//X軸は共通
	ybillCameraAxisX = cameraAxisX;
	//Y軸はワールド座標系のY軸
	ybillCameraAxisY = XMVector3Normalize(upVector);
	//Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = XMVector3Cross(cameraAxisX, cameraAxisY);

	////Y軸回りビルボード行列
	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion
	//カメラシェイクアップデート
	ShakeUpdate();

	//透視投影行列の計算
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),
		aspectRatio,
		0.1f, 1000.0f
	);

	
}

XMMATRIX Camera::GetMatView()
{
	return matView;
}

XMMATRIX Camera::GetMatProjection()
{
	return matProjection;
}

void Camera::ShakeSet(int time, float power)
{
	shakeTime = time;
	maxShakeTime = shakeTime;
	power_ = power;
	isShake = true;
}

void Camera::ShakeUpdate()
{
	XMFLOAT3 shakeDist{};
	Vector2 dist;
	if (shakeTime > 0) {
		shakeTime--;
		//乱数シード生成器
		std::random_device seed_gen;
		//メルセンヌ・ツイスターの乱数エンジン
		std::mt19937_64 engine(seed_gen());
		//振動の大きさ
		if (shakeTime > maxShakeTime * 0.8f) {
			dist = { -(power_ * 0.8f) , power_ * 0.8f };
		}
		else if (shakeTime > maxShakeTime * 0.6f) {
			dist = { -(power_ * 0.5f),power_ * 0.5f };
		}
		else if (shakeTime > maxShakeTime * 0.4f) {
			dist = { -(power_ * 0.2f), power_ * 0.2f };
		}
		else if (shakeTime > maxShakeTime * 0.2f) {
			dist = { -(power_ * 0.05f), power_ * 0.05f };
		}
		std::uniform_real_distribution<float> transDistX(dist.x, dist.y);
		std::uniform_real_distribution<float> transDistY(dist.x, dist.y);

		shakeDist.x = transDistX(engine);
		shakeDist.y = transDistY(engine);

		eye = originalPos + shakeDist;
	}
	else {
		SetOriginalPos();
		//元の座標を代入する
		//if (isShake == true) {
			eye = originalPos;
			isShake = false;
		//}
		
	}
}

void Camera::SetOriginalPos()
{
	originalPos = eye;
}

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3 v1, const DirectX::XMFLOAT3 v2)
{
	DirectX::XMFLOAT3 result{};
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

