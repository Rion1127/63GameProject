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
	//プレイヤーと床
	void PlayerToFloor();
	//敵と床
	void EnemyToFloor();
	//敵ロックオン
	void EnemyLockOn();
	//プレイヤーと敵押し出し
	void PlayerToEnemy();
	//プレイヤー攻撃と敵
	void PlayerAttackToEnemy();
	//敵攻撃とプレイヤー
	void EnemyAttackToPlayer();
};

