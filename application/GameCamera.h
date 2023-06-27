#pragma once
#include "Camera.h"
#include "player.h"
class GameCamera {
private:
	std::unique_ptr<Camera> camera_;
	Player* player_;
	Controller* controller_ = nullptr;
public:
	GameCamera();
	void CUpdate();
public:
	void SetPlayer(Player* player) { player_ = player; }
public:
	Camera* GetCamera() { return camera_.get(); }

};