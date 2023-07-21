#pragma once
#include "Floor.h"
#include "Collision.h"
class Stage
{
private:
	std::unique_ptr<Floor> floor_;
public:
	Stage();

	void Update();

	void Draw();

};

