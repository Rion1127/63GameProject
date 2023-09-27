#pragma once
#include "Sprite.h"
#include "Timer.h"
class Particle2DSelect
{
private:
	std::unique_ptr<Sprite> sprite_;

	Vector2 pos_;
	Vector2 endPos_;
	Vector2 offsetPos_;

	TimerFloat timer_;

	bool isInitPos_;
public:
	Particle2DSelect();
	void Update();
	void Draw();
public:
	void SetPos(Vector2 pos);
};

