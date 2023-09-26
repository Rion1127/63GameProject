#pragma once
#include "Camera.h"
#include  "Player.h"
#include "ColosseumSystem.h"

struct DeadZoneValue {
	int32_t x;
	int32_t y;
};

struct CameraSpeed {
	float x;
	float y;
};

enum class GetOutEnemy {
	Right,
	Left,
	Up,
	Down,
	Middle,
	LookBack
};

enum class GameCameraMode {
	NORMAL,
	CLEAR
};

class GameCamera {
private:
	std::unique_ptr<Camera> camera_;
	Player* player_;
	ColosseumSystem* clsumSystem_;
	
	Vector3 moveDist{};	//球面座標

	DeadZoneValue deadZone_;
	CameraSpeed transSpeed_;
	CameraSpeed lockOnCameraspeed_;
	//カメラの最終地点
	Vector3 endEyePos_;
	Vector3 endTargetPos_;
	Vector3 endRot_;

	float cameraSpeed_;
	bool putOnCamera_;	//ロックオン時カメラを戻すフラグ
	GetOutEnemy getOutWay;
	GameCameraMode gameCameraMode_;
private:
	//クリア時のカメラワークに使用する変数
	float frontDist_;
	Timer timer_;
	Vector3 offsetPos_;
	Vector3 randOffsetPos_;
public:
	GameCamera();
	void Update(CameraMode cameraMode);
	void UpdateCameraPos();
	void UpdateLookAT();
	void Reset();
private:
	GetOutEnemy GetOutScreenEnemy(const Vector2& screenPos,const Vector2& winSize,IEnemy* enemy);
public:
	void SetPlayer(Player* player) { player_ = player; }
	void SetColosseumSystem(ColosseumSystem* clsumSystem) { clsumSystem_ = clsumSystem; }
public:
	Camera* GetCamera() { return camera_.get(); }

};