#include "IActor.h"
#include "Util.h"

void IActor::ObjUpdate()
{
	addVec_ += gravity_.GetGravityValue();
	addVec_ += knockVec_;
	//ノックバックのベクトルを0にしていく
	MoveTo({ 0,0,0 }, 0.05f, knockVec_);

	obj_->WT_.position_ += addVec_;
	obj_->Update();
}
