#pragma once
#include "Object3d.h"
#include "Player.h"
#include "Collision.h"
class IEnemy
{
protected:
	static Player* splayer_;
	//ƒ‚ƒfƒ‹
	std::unique_ptr<Object3d> model_;
public:
	static void SetPlayer(Player* player) { splayer_ = player; }

	void Update();
	void Draw();
protected:
	virtual void MoveUpdate() = 0;
};

