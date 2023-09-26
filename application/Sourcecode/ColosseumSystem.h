#pragma once
#include "Player.h"
#include "EnemyManager.h"
#include <list>
#include <array>
#include "Timer.h"
#include "Sprite.h"

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
	std::unique_ptr<Sprite> retrySprite_;
	std::unique_ptr<Sprite> titleSprite_;

	uint32_t maxRoundNum_;
	uint32_t roundNum_;

	bool isStart_;//ready�\����
	bool isNext_;//���̃��E���h�ֈړ�����
	bool isClearCamera_;//���̃��E���h�̓G��0�ɂ����ꍇtrue
	bool isReset_;//���Z�b�g�t���O
	bool isClear_;//�S�Ẵ��E���h���I��

	Timer clearBlankTimer_;
	Timer blindTimer_;
	Timer gameClearBlankTimer_;
	
	SelectType selectType_;
	ClearType clearType_;
public:
	ColosseumSystem();
	void Update();
	void DrawSprite();

	void Reset();
private:
	void ClearUpdate();
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

