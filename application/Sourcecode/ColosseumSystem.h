#pragma once
#include "IActor.h"
#include <list>
#include "Timer.h"
#include "Sprite.h"

class ColosseumSystem
{
private:
	std::list<IActor*> enemys_;
	IActor* player_;

	bool isStart_;
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
	void SetEnemy(IActor* actor) { enemys_.emplace_back(actor); };
};

