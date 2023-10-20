#pragma once
#include "Collision.h"
#include "Timer.h"
#include "Object3d.h"

/**
 * @file Guard.h
 * @brief プレイヤーのガード処理をしている
 */

class Player;

class GuardClass
{
private:
	struct EnableTime {
		int32_t start;
		int32_t end;
	};
private:
	Player* player_;
	Sphere col_;
	Timer timer_;

	float frontDist_;

	std::unique_ptr<Object3d> colObj_;

	bool isGurdNow_;
	//ガードを開始してから有効になるまで
	EnableTime enabledTime_;

public:
	GuardClass();
	void Init();
	void Update();
	void DrawDebug();
	//ガード成功時の処理
	void GuardHit();
public:
	void SetPlayer(Player* player) { player_ = player; }
	void SetisGurdNow_(bool flag) {
		timer_.SetTime(timer_.GetLimitTimer());
		isGurdNow_ = flag;
	}
public:
	bool GetIsGurdNow() { return isGurdNow_; }
	Sphere GetCol() { return col_; }

};

