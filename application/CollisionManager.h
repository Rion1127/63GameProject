#pragma once
#include "Player.h"
#include "Floor.h"
#include "EnemyManager.h"
class CollisionManager
{
private:
	Player* player_;
	Floor* floor_;
	EnemyManager* enemyManager_;
public:
	void Update();

public:
	void SetPlayer(Player* player) { player_ = player; }
	void SetFloor(Floor* floor) { floor_ = floor; }
	void SetEnemys(EnemyManager* enemy) { enemyManager_ = enemy; }
private:
	//ÉvÉåÉCÉÑÅ[Ç∆è∞
	void PlayerToFloor();
	//ìGÇ∆è∞
	void EnemyToFloor();
	
};

