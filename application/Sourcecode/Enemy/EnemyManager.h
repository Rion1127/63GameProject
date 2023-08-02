#pragma once
#include "IEnemy.h"
#include <vector>
#include <list>
#include <array>
#include "Timer.h"
#include "EnemyHPGauge.h"
#include "mInput.h"
#include "Player.h"
class EnemyManager
{
private:
	std::list<std::unique_ptr<IEnemy>> enemys_;

	Player* player_;
	IEnemy* lockOnEnemy_;	//���b�N�I�����Ă���G
	Timer lockOnobjTimer_;
	float lockOnobjRot;
	float addRot;
	EnemyHPGauge hpGauge_;
	std::array<std::unique_ptr<Sprite>,2> lockOnSprite_;
public:
	EnemyManager();
	void PreUpdate();
	void PostUpdate();
	void Draw();
	void SpriteDraw();
	void Damage();

	void Reset();
private:
	void LockOnSpriteUpdate();
public:
	std::list<std::unique_ptr<IEnemy>>* GetEnemy() { return &enemys_; };
	IEnemy* GetLockOnEnemy() { return lockOnEnemy_; }
public:
	void SetPlayer(Player* player) { player_ = player; }
public:
};
