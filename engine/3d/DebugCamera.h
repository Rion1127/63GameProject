#pragma once
#include "Vector3.h"
#include "mInput.h"
#include "Camera.h"

/**
 * @file DebugCamera.h
 * @brief デバッグ用のカメラを管理している
 */

enum class DebugCameraMode {
	Normal,
	Trans_Zero
};

class DebugCamera
{
private:
	MouseInput* mInput_ = nullptr;

	Camera camera_;

	Vector3 dist_{};
	Vector3 moveDist_{};	//球面座標
	Vector3 cameraTrans_{};//平行移動座標

	//正面・横・上ベクトル
	Vector3 frontVec_{};
	Vector3 sideVec_{};
	Vector3 upVec_{};

	float frontdist_;

	DebugCameraMode debugCameraMode_;

public:
	DebugCamera();

	void Update();
private:
	void CameraMove();
public:
	Camera* GetCamera() { return &camera_; }
public:
	void SetDebugCameraMode(DebugCameraMode mode) { debugCameraMode_ = mode; }
	void SetFrontDist(float value) { frontdist_ = value; }
};

