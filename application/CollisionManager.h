#pragma once
#include "Player.h"
class CollisionManager
{
private:
	Player* player_;

public:
	void Update();

public:
	void SetPlayer(Player* player) { player_ = player; }
};

