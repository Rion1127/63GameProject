#include "IActor.h"
#include "Util.h"

void IActor::ObjUpdate()
{
	
	addVec_ += gravity_.GetGravityValue();
	addVec_ += knockVec_;
	//�m�b�N�o�b�N�̃x�N�g����0�ɂ��Ă���
	MoveTo({ 0,0,0 }, knockDecreaseValue, knockVec_);

	if (isCanMove_ == false)
	{
		addVec_ = { 0,0,0 };
	}
	obj_->WT_.position_ += addVec_;
	obj_->Update();
}
