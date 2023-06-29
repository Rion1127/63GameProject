#pragma once
#include <vector>
#include "Object3d.h"
#include "Collision.h"
#include "mInput.h"


struct AttackCol {
	Object3d colObj_;
	Sphere col_;
};

class IAttack
{
protected:
	std::vector<std::unique_ptr<AttackCol>> attackCol_;
	Controller* controller_ = nullptr;
public:
	IAttack(size_t colNum);

	virtual void Update();
	virtual void DrawCol();
protected:
	virtual void MoveUpdate() = 0;
};

