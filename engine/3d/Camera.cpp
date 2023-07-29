#include <d3dx12.h>
#include <random>
#include "Util.h"
#include "Camera.h"
#include <imgui.h>

Camera* Camera::scurrent_ = nullptr;

Camera::Camera()
{
	aspectRatio_ = WinAPI::GetWindowSize().x / WinAPI::GetWindowSize().y;

	eye_ = { 0,0,-10 };
	target_ = { 0,0,0 };
	up_ = { 0,1,0 };

	UpdateLookAt();

	WT_.SetRotation(rot_);
	WT_.Update();
}

void Camera::SetEyePos(float x, float y, float z)
{
	eye_.x = x;
	eye_.y = y;
	eye_.z = z;
}

void Camera::SetEyePos(Vector3 pos)
{
	if (isShake_)
	{
		originalPos_.x = pos.x;
		originalPos_.y = pos.y;
		originalPos_.z = pos.z;
	}
	else if (isShake_ == false)
	{
		eye_.x = pos.x;
		eye_.y = pos.y;
		eye_.z = pos.z;
	}
}

void Camera::SetTarget(float x, float y, float z)
{
	target_.x = x;
	target_.y = y;
	target_.z = z;
}

void Camera::SetTarget(Vector3 pos)
{
	target_.x = pos.x;
	target_.y = pos.y;
	target_.z = pos.z;
}

void Camera::SetUpVec(float x, float y, float z)
{
	up_.x = x;
	up_.y = y;
	up_.z = z;
}

void Camera::SetUpVec(Vector3 upVec)
{
	up_.x = upVec.x;
	up_.y = upVec.y;
	up_.z = upVec.z;
}

void Camera::MoveTo(Vector3 goal, float speed)
{
	Vector3 dir = goal - eye_;
	float dirLength = dir.length();
	if (dirLength < speed * speed)
	{
		eye_.x = goal.x;
		eye_.y = goal.y;
		eye_.z = goal.z;
		return;
	}
	eye_.x = eye_.x + dir.SetLength(speed).x;
	eye_.y = eye_.y + dir.SetLength(speed).y;
	eye_.z = eye_.z + dir.SetLength(speed).z;
}


void Camera::UpdateLookAt()
{
	//���_���W
	Vector3 eyePosition = eye_;
	//�����_���W
	Vector3 targetPosition = target_;
	//�i���́j�����
	Vector3 upVector = up_;

	//�J����Z���i���������j
	Vector3 cameraAxisZ = targetPosition - eyePosition;
	//�x�N�g���𐳋K��
	cameraAxisZ = cameraAxisZ.normalize();
	//�J������X���i�E�����j
	Vector3 cameraAxisX;
	//X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = upVector.cross(cameraAxisZ);
	//�x�N�g���𐳋K��
	cameraAxisX = cameraAxisX.normalize();
	//�J������Y���W�i������j
	Vector3 cameraAxisY;
	//Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = cameraAxisZ.cross(cameraAxisX);
	//�J������]�s��
	Matrix4 matCameraRot{};
	//�J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.m[0][0] = cameraAxisX.x;
	matCameraRot.m[0][1] = cameraAxisX.y;
	matCameraRot.m[0][2] = cameraAxisX.z;

	matCameraRot.m[1][0] = cameraAxisY.x;
	matCameraRot.m[1][1] = cameraAxisY.y;
	matCameraRot.m[1][2] = cameraAxisY.z;

	matCameraRot.m[2][0] = cameraAxisZ.x;
	matCameraRot.m[2][1] = cameraAxisZ.y;
	matCameraRot.m[2][2] = cameraAxisZ.z;

	matCameraRot.m[3][0] = 0;
	matCameraRot.m[3][1] = 0;
	matCameraRot.m[3][2] = 0;
	matCameraRot.m[3][3] = 1;
	//�]�u���đ��

	matView_.m[0][0] = matCameraRot.m[0][0];
	matView_.m[1][0] = matCameraRot.m[0][1];
	matView_.m[2][0] = matCameraRot.m[0][2];
	matView_.m[3][0] = matCameraRot.m[0][3];

	matView_.m[0][1] = matCameraRot.m[1][0];
	matView_.m[1][1] = matCameraRot.m[1][1];
	matView_.m[2][1] = matCameraRot.m[1][2];
	matView_.m[3][1] = matCameraRot.m[1][3];

	matView_.m[0][2] = matCameraRot.m[2][0];
	matView_.m[1][2] = matCameraRot.m[2][1];
	matView_.m[2][2] = matCameraRot.m[2][2];
	matView_.m[3][2] = matCameraRot.m[2][3];

	matView_.m[0][3] = matCameraRot.m[3][0];
	matView_.m[1][3] = matCameraRot.m[3][1];
	matView_.m[2][3] = matCameraRot.m[3][2];
	matView_.m[3][3] = matCameraRot.m[3][3];

	//���_���W��-1��q�������W
	Vector3 reverseEyePosition = eyePosition * -1;
	//�r���[�s��ɕ��s�ړ�������ݒ�
	matView_.m[3][0] = cameraAxisX.dot(reverseEyePosition);
	matView_.m[3][1] = cameraAxisY.dot(reverseEyePosition);
	matView_.m[3][2] = cameraAxisZ.dot(reverseEyePosition);
	matView_.m[3][3] = 1.f;
	//�S�����r���{�[�h�s��
	matBillboard_.m[0][0] = cameraAxisX.x;
	matBillboard_.m[0][1] = cameraAxisX.y;
	matBillboard_.m[0][2] = cameraAxisX.z;
	matBillboard_.m[0][3] = 0;

	matBillboard_.m[1][0] = cameraAxisY.x;
	matBillboard_.m[1][1] = cameraAxisY.y;
	matBillboard_.m[1][2] = cameraAxisY.z;
	matBillboard_.m[1][3] = 0;

	matBillboard_.m[2][0] = cameraAxisZ.x;
	matBillboard_.m[2][1] = cameraAxisZ.y;
	matBillboard_.m[2][2] = cameraAxisZ.z;
	matBillboard_.m[2][3] = 0;

	matBillboard_.m[3][0] = 0;
	matBillboard_.m[3][1] = 0;
	matBillboard_.m[3][2] = 0;
	matBillboard_.m[3][3] = 1;

	////Y������r���{�[�h�s��
	////�J����XYZ��
	//XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;
	////X���͋���
	//ybillCameraAxisX = cameraAxisX;
	////Y���̓��[���h���W�n��Y��
	//ybillCameraAxisY = XMVector3Normalize(upVector);
	////Z����X����Y���̊O�ςŋ��܂�
	//ybillCameraAxisZ = XMVector3Cross(cameraAxisX, cameraAxisY);

	//////Y�����r���{�[�h�s��
	//matBillboardY_.r[0] = ybillCameraAxisX;
	//matBillboardY_.r[1] = ybillCameraAxisY;
	//matBillboardY_.r[2] = ybillCameraAxisZ;
	//matBillboardY_.r[3] = XMVectorSet(0, 0, 0, 1);

	//�J�����V�F�C�N�A�b�v�f�[�g
	ShakeUpdate();

	UpdateMatProjection();
}

void Camera::UpdateLookTo()
{
	WT_.SetRotation(rot_);
	WT_.Update();
	//�J�������W��Y���AZ�����擾
	Vector3 pos = eye_;
	Vector3 axisY = WT_.GetMatWorld().GetAxisY();
	Vector3 axisZ = WT_.GetMatWorld().GetAxisZ();
	//�P�ʍs��ɏ�����
	matView_.UnitMatrix();
	//Z����Y�����擾
	Vector3 xAxisVec = axisY.cross(axisZ).normalize();
	Vector3 yAxisVec = axisZ.cross(xAxisVec).normalize();
	//���s�ړ�������ݒ�
	Vector3 transPos = {
		-pos.dot(xAxisVec.normalize()),
		-pos.dot(yAxisVec.normalize()),
		-pos.dot(axisZ.normalize())
	};

	matView_ = {
		xAxisVec.x,yAxisVec.x,axisZ.x	,0.f,
		xAxisVec.y,yAxisVec.y,axisZ.y	,0.f,
		xAxisVec.z,yAxisVec.z,axisZ.z	,0.f,
		transPos.x,transPos.y,transPos.z,1.f,
	};
}

void Camera::Update(CameraMode mode)
{
	if (CameraMode::LookAT == mode)
	{
		UpdateLookAt();
	}
	else if (CameraMode::LookTo == mode)
	{
		UpdateLookTo();
	}
}

Matrix4 Camera::GetMatView()
{
	return matView_;
}

Matrix4 Camera::GetMatProjection()
{
	return matProjection_;
}

void Camera::ShakeSet(uint32_t time, float power)
{
	shakeTime_ = time;
	maxShakeTime_ = shakeTime_;
	power_ = power;
	isShake_ = true;
}

void Camera::ShakeUpdate()
{
	//XMFLOAT3 shakeDist{};
	//Vector2 dist;
	//if (shakeTime_ > 0) {
	//	shakeTime_--;
	//	//�����V�[�h������
	//	std::random_device seed_gen;
	//	//�����Z���k�E�c�C�X�^�[�̗����G���W��
	//	std::mt19937_64 engine(seed_gen());
	//	//�U���̑傫��
	//	if (shakeTime_ > maxShakeTime_ * 0.8f) {
	//		dist = { -(power_ * 0.8f) , power_ * 0.8f };
	//	}
	//	else if (shakeTime_ > maxShakeTime_ * 0.6f) {
	//		dist = { -(power_ * 0.5f),power_ * 0.5f };
	//	}
	//	else if (shakeTime_ > maxShakeTime_ * 0.4f) {
	//		dist = { -(power_ * 0.2f), power_ * 0.2f };
	//	}
	//	else if (shakeTime_ > maxShakeTime_ * 0.2f) {
	//		dist = { -(power_ * 0.05f), power_ * 0.05f };
	//	}
	//	std::uniform_real_distribution<float> transDistX(dist.x, dist.y);
	//	std::uniform_real_distribution<float> transDistY(dist.x, dist.y);

	//	shakeDist.x = transDistX(engine);
	//	shakeDist.y = transDistY(engine);

	//	eye_ = originalPos_ + shakeDist;
	//}
	//else {
	//	SetOriginalPos();
	//	//���̍��W��������
	//	//if (isShake == true) {
	//		eye_ = originalPos_;
	//		isShake_ = false;
	//	//}
	//
	//}
}

void Camera::SetOriginalPos()
{
	originalPos_ = eye_;
}

void Camera::UpdateMatProjection()
{
	float scaleY = 1.f / tanf(Radian(45) / 2);
	float scaleX = 1.f / tanf(Radian(45) / 2) / aspectRatio_;
	float scaleZ = 1.f / (1000.0f - 0.1f) * 1000.0f;
	float TransZ = -0.1f / (1000.0f - 0.1f) * 1000.0f;

	matProjection_.m[1][1] = scaleY;
	matProjection_.m[0][0] = scaleX;
	matProjection_.m[2][2] = scaleZ;
	matProjection_.m[3][2] = TransZ;
	matProjection_.m[2][3] = 1;
}