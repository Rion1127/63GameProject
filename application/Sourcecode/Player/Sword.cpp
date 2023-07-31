#include "Sword.h"
#include "util.h"
#include <imgui.h>

Sword::Sword()
{
	obj_ = MakeUnique<Object3d>();
	obj_->SetModel(Model::CreateOBJ_uniptr("sword"));
	obj_->SetScale({ 0.3f,0.3f, 0.3f });
}

void Sword::Update()
{
	if (attackManager_->GetNowAttack() != nullptr) {
		Vector3 pos = attackManager_->GetNowAttack()->GetAttackCol()->at(0)->col_.center;
		localPos_ = pos - playerObj_->WT_.position_;
		obj_->SetPos(pos);
	}

	Vector3 PtoSVec = obj_->WT_.position_ - playerObj_->WT_.position_;
	PtoSVec = PtoSVec.normalize();

	Vector2 PtoSVec2 = {
		PtoSVec.x,
		PtoSVec.y,
	};

	obj_->WT_.SetRotType(RotType::Quaternion);
	
	obj_->WT_.quaternion_ = DirectionToDirection(Vector3(0, 1, 0), PtoSVec);

	obj_->Update();
}

void Sword::Draw()
{
	if (attackManager_->GetNowAttack() != nullptr) {
		obj_->Draw();
	}
}
