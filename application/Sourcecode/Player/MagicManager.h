#pragma once
#include "IAttack.h"
#include "IBullet.h"
class MagicManager
{
private:
	std::vector<IAttack> magics_;
	//std::vector<IAttack> magics_;

public:
	MagicManager();
	void Update();

	void Draw();
};

