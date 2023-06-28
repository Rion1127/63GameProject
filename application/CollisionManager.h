#pragma once
#include "Player.h"
#include "Floor.h"
class CollisionManager
{
private:
	Player* player_;
	Floor* floor_;
public:
	void Update();

public:
	void SetPlayer(Player* player) { player_ = player; }
	void SetFloor(Floor* floor) { floor_ = floor; }
};

