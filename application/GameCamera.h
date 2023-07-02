#pragma once
#include "Camera.h"
#include "player.h"

struct DeadZoneValue {
	size_t x;
	size_t y;
};
class GameCamera {
private:
	std::unique_ptr<Camera> camera_;
	Player* player_;
	Controller* controller_ = nullptr;

	DeadZoneValue deadZone_;
	//カメラの最終地点
	Vector3 endEyePos_;
	Vector3 endTargetPos_;
public:
	GameCamera();
	void Update();
public:
	void SetPlayer(Player* player) { player_ = player; }
public:
	Camera* GetCamera() { return camera_.get(); }

};