#pragma once
#include "IActor.h"
#include "EnemyManager.h"
#include <list>
#include "Timer.h"
#include "Sprite.h"


class ColosseumSystem
{
private:
	EnemyManager* enemyManager_;
	IActor* player_;

	bool isStart_;
	bool isClear_;
	std::unique_ptr<Sprite> readySprite_;
	std::unique_ptr<Sprite> goSprite_;
	Timer easeTimer_;
	Timer displayTimer_;
public:
	ColosseumSystem();
	void Update();
	void DrawSprite();
public:
	void SetPlayer(IActor* actor) { player_ = actor; };
	void SetEnemy(EnemyManager* actor) { enemyManager_ = actor; };
};

