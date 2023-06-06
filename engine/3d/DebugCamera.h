#pragma once
#include "Vector3.h"
#include "mInput.h"
#include "Camera.h"
#include <DirectXMath.h>
using namespace DirectX;
class DebugCamera
{
public:
	void DebugCameraIni();
	void Update();


	Camera* GetViewProjection();

private:
	MouseInput* mInput_ = nullptr;

	void CameraMove();

	Camera viewProjection;

	Vector3 dist{};
	Vector3 moveDist{};	//���ʍ��W
	Vector3 cameraTrans{};//���s�ړ����W

	//���ʁE���E��x�N�g��
	Vector3 frontVec{};
	Vector3 sideVec{};
	Vector3 upVec{};

	

	float frontdist;

};

