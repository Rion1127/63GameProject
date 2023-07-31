#pragma once
#include "Sprite.h"
#include <vector>
class UIOperation
{
private:
	std::vector<std::unique_ptr<Sprite>> operationUI_;
	std::vector<std::unique_ptr<Sprite>> textUI_;
public:
	UIOperation();
	void Update();

	void Draw();
};

