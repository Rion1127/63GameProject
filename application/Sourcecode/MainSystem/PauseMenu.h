#pragma once
#include "Sprite.h"
#include <array>
#include "Timer.h"
#include "Particle2DSelect.h"

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
		Ease,
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
	void SetIsActive(bool flag) { isActive_ = flag; }
};

class PauseSelectSprite {
private:
	std::array<std::unique_ptr<Sprite>, 3> frameSprite_;
	std::array<std::unique_ptr<Sprite>, 3> texSprite_;

	Particle2DSelect selectParticle_;
public:
	PauseSelectSprite();
	void Update(int32_t index);
	void Draw();

};



class PauseMenu
{
private:
	enum SelectType {
		Continue,
		Config,
		Title,
	};
private:
	std::unique_ptr<Sprite> backSprite_;
	std::unique_ptr<PauseSprite> pauseSprite_;

	PauseSelectSprite pauseSelectSprite_;

	SelectType selectType_;
	bool isPause_;
	bool isConfig_;
public:
	PauseMenu();
	void Update();
	void Draw();
public:
	bool GetIsPause() { return isPause_; }
};

