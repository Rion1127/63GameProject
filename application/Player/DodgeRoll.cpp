#include "DodgeRoll.h"
#include "Util.h"

void DodgeRoll::Begin(Vector2 frontVec)
{
	speed_ = 1.2f;
	dodgetimer_.SetLimitTime(60);
	dodgeVec_ = {
		frontVec.x * speed_,
		0,
		frontVec.y * speed_,
	};
}

void DodgeRoll::Update()
{
	MoveTo({0,0,0}, 0.15f, dodgeVec_);
}
