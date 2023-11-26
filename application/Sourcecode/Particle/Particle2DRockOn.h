#pragma once
#include "Sprite.h"
#include "Timer.h"
class Particle2DRockOn
{
private:
	std::unique_ptr<Sprite> sprite_;

	Vector2 startPos_;
	Vector2 endPos_;

	Vector2 LUPos_;
	Vector2 RUPos_;
	Vector2 LDPos_;
	Vector2 RDPos_;

	Timer startPosTimer_;
	Timer endPosTimer_;
	Timer delayTimer_;

	float upLength_;
	float downLength_;

	bool isStart_;
public:
	Particle2DRockOn();
	void Reset();
	void Update();
	void Draw();
public:
	void SetStartPos(Vector2 pos) { startPos_ = pos; }
	void SetEndPos(Vector2 pos) { endPos_ = pos; }
	void SetIsStart(bool flag) { isStart_ = flag; }
public:
	bool GetIsStaert() { return isStart_; }
};

