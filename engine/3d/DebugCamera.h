#pragma once
#include "Vector3.h"
#include "mInput.h"
#include "Camera.h"
class DebugCamera
{
private:
	MouseInput* mInput_ = nullptr;

	Camera camera_;

	Vector3 dist_{};
	Vector3 moveDist_{};	//���ʍ��W
	Vector3 cameraTrans_{};//���s�ړ����W

	//���ʁE���E��x�N�g��
	Vector3 frontVec_{};
	Vector3 sideVec_{};
	Vector3 upVec_{};

	float frontdist_;
public:
	DebugCamera();

	void Update();
private:
	void CameraMove();
public:
	Camera* GetCamera() { return &camera_; }
};

