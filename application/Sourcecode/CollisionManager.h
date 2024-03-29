#pragma once
#include "Player.h"
#include "Stage.h"
#include "EnemyManager.h"
#include "HitStop.h"
#include "GameCamera.h"

/**
 * @file CollisionManager.h
 * @brief 当たり判定をまとめているクラス
 */

class CollisionManager
{
private:
	Player* player_;
	Stage* stage_;
	EnemyManager* enemyManager_;
	GameCamera* gameCamera_;
	bool isHitStop_;
	float hitStopTimer_;

	float shakePower_;
	float shakeTimer_;

	HitStop hitstop_;

	int32_t lockOnEnemyIndex_;
private:
	//パーティクル用
	Timer wallHitTimer_;
public:
	void Update();
	void DrawImGui();
public:
	CollisionManager();
	void SetPlayer(Player* player) { player_ = player; }
	void SetFloor(Stage* floor) { stage_ = floor; }
	void SetEnemys(EnemyManager* enemy) { enemyManager_ = enemy; }
	void SetCamera(GameCamera* camera) { gameCamera_ = camera; }
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
	//敵同士の押し出し
	void EnemyToEnemy();
	//プレイヤー攻撃と敵
	void PlayerAttackToEnemy();
	//敵攻撃とプレイヤー
	void EnemyAttackToPlayer();
	//敵弾とプレイヤー
	void EnemyBulletToPlayer();
	//敵弾と床
	void EnemyBulletToFloor();
};

