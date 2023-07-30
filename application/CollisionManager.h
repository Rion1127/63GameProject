#pragma once
#include "Player.h"
#include "Stage.h"
#include "EnemyManager.h"
class CollisionManager
{
private:
	Player* player_;
	Stage* stage_;
	EnemyManager* enemyManager_;
public:
	void Update();

public:
	void SetPlayer(Player* player) { player_ = player; }
	void SetFloor(Stage* floor) { stage_ = floor; }
	void SetEnemys(EnemyManager* enemy) { enemyManager_ = enemy; }
private:
	//プレイヤーと床
	void PlayerToFloor();
	//プレイヤーと壁
	void PlayerToWall();
	//敵と床
	void EnemyToFloor();
	//敵と床
	void EnemyToWall();
	//敵ロックオン
	void EnemyLockOn();
	//プレイヤーと敵押し出し
	void PlayerToEnemy();
	//プレイヤー攻撃と敵
	void PlayerAttackToEnemy();
	//敵攻撃とプレイヤー
	void EnemyAttackToPlayer();
};

