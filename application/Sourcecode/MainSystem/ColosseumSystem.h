#pragma once
#include "Player.h"
#include "EnemyManager.h"
#include <list>
#include <array>
#include "Timer.h"
#include "Sprite.h"
#include "Particle2DSelect.h"

class ClearSprite {
private:
	std::unique_ptr<Sprite> sprite_;

	Timer shineTimer_;

	bool isStart_;
public:
	ClearSprite();
	void Update(Timer timer);
	void Draw();
	void SetIsStart(bool flag) { isStart_ = flag; }
	void Reset();
};

class ReadyGoSprite {
private:
	std::unique_ptr<Sprite> readySprite_;
	std::array<std::unique_ptr<Sprite>, 2> readyShapeSprite_;
	std::unique_ptr<Sprite> goSprite_;
	std::unique_ptr<Sprite> goShapeSprite_;

	Timer readyTimer_;
	Timer rdyEaseTimer_;
	Timer displayTimer_;

	bool isStart_;
public:
	ReadyGoSprite();
	void Update(bool flag);
	void Draw(bool flag);
	void Reset();
private:
	void ReadySpriteUpdate();
	void GoSpriteUpdate();
public:
	void SetIsStart(bool flag) { isStart_ = flag; }
public:
	Timer GetReadyTimer() { return readyTimer_; }
};

class SelectSprite {
private:
	std::array<std::unique_ptr<Sprite>, 2> frameSprite_;
	std::array<std::unique_ptr<Sprite>, 2> texSprite_;

	Particle2DSelect selectParticle_;
public:
	SelectSprite();
	void Update(uint32_t index);
	void Draw();

};


enum class ClearType {
	NextRound,
	GameClear
};

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

	ClearSprite clearSprite_;
	ReadyGoSprite readyGoSprite_;
	std::unique_ptr<Sprite> blindSprite_;

	SelectSprite selectSprite_;

	uint32_t maxRoundNum_;
	uint32_t roundNum_;

	bool isStart_;//ready表示中
	bool isNext_;//次のラウンドへ移動する
	bool isClearCamera_;//そのラウンドの敵を0にした場合true
	bool isReset_;//リセットフラグ
	bool isClear_;//全てのラウンドが終了

	Timer clearBlankTimer_;
	Timer blindTimer_;
	Timer gameClearBlankTimer_;

	SelectType selectType_;
	ClearType clearType_;
public:
	ColosseumSystem();
	void Update();
	void ClearUpdate();
	void DrawSprite();

	void Reset();
private:
public:
	void SetPlayer(Player* actor) { player_ = actor; };
	void SetEnemy(EnemyManager* actor) { enemyManager_ = actor; };
	void SetIsReset(bool flag) { isReset_ = flag; }
	void SetMaxRoundNum(uint32_t num) { maxRoundNum_ = num; }
public:
	bool GetIsNext() { return isNext_; }
	bool GetIsClear() { return isClear_; }
	bool GetIsReset() { return isReset_; }
	bool GetIsClearCamera() { return isClearCamera_; }
	uint32_t GetRoundNum() { return roundNum_; }
	ClearType GetClearType() { return clearType_; }

	Timer GetClearBlankTimer() { return clearBlankTimer_; }
	Timer GetGameClearBlankTimer() { return gameClearBlankTimer_; }
};

