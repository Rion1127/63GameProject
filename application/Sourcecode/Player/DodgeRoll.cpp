#include "DodgeRoll.h"
#include "Util.h"

void DodgeRoll::Begin(Vector2 frontVec)
{
	speed_ = 1.1f;
	dodgetimer_.SetLimitTime(60);
	dodgeVec_ = {
		frontVec.x * speed_,
		0,
		frontVec.y * speed_,
	};
	isDodge_ = true;
}

void DodgeRoll::Update()
{
	dodgetimer_.AddTime(1);
	MoveTo({0,0,0}, 0.15f, dodgeVec_);

	if (dodgetimer_.GetIsEnd()) {
		isDodge_ = false;
	}
}
