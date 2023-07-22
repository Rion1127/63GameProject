#pragma once
#include "Collision.h"
#include "Timer.h"
#include "Object3d.h"

class Player;

class Guard
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
	//�K�[�h���J�n���Ă���L���ɂȂ�܂�
	EnableTime enabledTime_;
public:
	Guard();
	void Init();
	void Update();
	void DrawDebug();
public:
	void SetPlayer(Player* player) { player_ = player; }
public:
	bool GetIsGurdNow() { return isGurdNow_; }
};

