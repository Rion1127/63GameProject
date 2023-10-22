#pragma once
#include "IEnemy.h"
#include <vector>
#include <list>
#include <array>
#include "Timer.h"
#include "EnemyHPGauge.h"
#include "mInput.h"
#include "Player.h"
#include "IBullet.h"
#include "HitStop.h"

/**
 * @file EnemyManager.h
 * @brief 全ての敵を管理するクラス
 */

class EnemyManager : public HitStop
{
private:
	std::list<std::unique_ptr<IEnemy>> enemys_;
	std::list<std::unique_ptr<IBullet>> bullets_;

	Player* player_;
	IEnemy* lockOnEnemy_;	//ロックオンしている敵
	Timer lockOnobjTimer_;
	float lockOnobjRot;
	float addRot;
	EnemyHPGauge hpGauge_;
	std::array<std::unique_ptr<Sprite>, 2> lockOnSprite_;
public:
	EnemyManager();
	void PreUpdate();
	void PostUpdate();
	void Draw();
	void SpriteDraw();
	void Damage();

	void BulletUpdate();
	void Reset();

	void DebugUpdate();
private:
	void LockOnSpriteUpdate();
public:
	std::list<std::unique_ptr<IEnemy>>* GetEnemy() { return &enemys_; };
	std::list<std::unique_ptr<IBullet>>* GetBullet() { return &bullets_; };
	IEnemy* GetLockOnEnemy() { return lockOnEnemy_; }
public:
	void SetPlayer(Player* player) { player_ = player; }
public:
};

