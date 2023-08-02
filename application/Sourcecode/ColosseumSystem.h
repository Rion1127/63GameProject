#pragma once
#include "Player.h"
#include "EnemyManager.h"
#include <list>
#include <array>
#include "Timer.h"
#include "Sprite.h"


class ColosseumSystem
{
private:
	enum class SelectType {
		Retry,
		Title
	};
private:
	EnemyManager* enemyManager_;
	Player* player_;

	std::unique_ptr<Sprite> readySprite_;
	std::array<std::unique_ptr<Sprite>,2> readyShapeSprite_;
	std::unique_ptr<Sprite> goSprite_;
	std::unique_ptr<Sprite> blindSprite_;
	std::unique_ptr<Sprite> clearSprite_;

	std::unique_ptr<Sprite> retrySprite_;
	std::unique_ptr<Sprite> titleSprite_;


	uint32_t maxRoundNum_;
	uint32_t roundNum_;

	bool isStart_;
	bool isNext_;
	bool isReset_;
	bool isClear_;
	Timer readyTimer_;
	Timer displayTimer_;
	Timer clearBlankTimer_;
	Timer blindTimer_;

	Timer rdyEaseTimer_;

	SelectType selectType_;
public:
	ColosseumSystem();
	void Update();
	void DrawSprite();

	void Reset();
private:
	void ReadySpriteUpdate();
	void ClearUpdate();
public:
	void SetPlayer(Player* actor) { player_ = actor; };
	void SetEnemy(EnemyManager* actor) { enemyManager_ = actor; };
	void SetIsReset(bool flag) { isReset_ = flag; }
	void SetMaxRoundNum(uint32_t num) { maxRoundNum_ = num; }
public:
	bool GetIsClear() { return isNext_; }
	bool GetIsReset() { return isReset_; }
	uint32_t GetRoundNum() { return roundNum_; }
};

