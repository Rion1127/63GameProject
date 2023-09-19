#pragma once
#include "Sprite.h"
#include <array>
#include "Timer.h"

struct PauseSprits {
	std::unique_ptr<Sprite> sprite;
	bool isActive;
	Timer timer;
	Vector2 easeStartPos_;
	Vector2 easeEndPos_;
};

class PauseSprite {
private:
	enum State {
		Collapse,
		Jump,
		Landing
	};
private:
	std::array<std::unique_ptr<PauseSprits>, 5> pauseSprite_;

	Vector2 pos_;
	Vector2 scale_;
	Timer timer_;
	uint32_t index_;
	bool isActive_;

	Timer effectTimer_;
	State state_;
	float gravity_;
public:
	PauseSprite();

	void Update();
	void Draw();

	void Reset();
public:
	void SetIsAvtive(bool flag) { isActive_ = flag; }
};

class PauseMenu
{
private:
	enum SelectType {
		Continue,
		Title
	};
private:
	std::unique_ptr<Sprite> backSprite_;
	std::unique_ptr<PauseSprite> pauseSprite_;
	std::unique_ptr<Sprite> continueSprite_;
	std::unique_ptr<Sprite> titleSprite_;

	SelectType selectType_;
	bool isPause_;
public:
	PauseMenu();
	void Update();
	void Draw();
public:
	bool GetIsPause() { return isPause_; }
};

