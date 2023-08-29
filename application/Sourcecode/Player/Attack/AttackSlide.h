#pragma once
#include "IAttack.h"
class AttackSlide :
    public IAttack
{
private:
	enum State {
		Slide,
		SpeedDown
	};
private:
	float frontDist_;
	State state_;
	float speed_;
public:
	AttackSlide(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;
};

