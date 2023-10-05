#include "DodgeRoll.h"
#include "Util.h"

void DodgeRoll::Begin(Vector2 frontVec)
{
	speed_ = 0.5f;
	dodgetimer_.SetLimitTime(30);
	dodgetimer_.Reset();
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
	MoveTo({0,0,0}, 0.03f, dodgeVec_);

	if (dodgetimer_.GetIsEnd()) {
		dodgetimer_.Reset();
		isDodge_ = false;
	}
}
