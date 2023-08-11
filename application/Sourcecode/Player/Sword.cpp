#include "Sword.h"
#include "util.h"
#include <imgui.h>
#include "ParticleHitAttack.h"

Sword::Sword()
{
	obj_ = MakeUnique<Object3d>();
	obj_->SetModel(Model::CreateOBJ_uniptr("sword"));
	obj_->SetScale({ 0.5f,0.5f, 0.5f });
	obj_->WT_.SetRotType(RotType::Quaternion);
}

void Sword::Update()
{
	//UŒ‚Žž‚ÌŒ•‚Ì“®‚«
	if (attackManager_->GetNowAttack() != nullptr) {
		//À•W
		Vector3 pos = attackManager_->GetNowAttack()->GetAttackCol()->at(0)->col_.center;
		localPos_ = pos - playerObj_->WT_.position_;
		obj_->SetPos(pos);
		nowPos_ = pos;
		//‰ñ“]î•ñ
		Vector3 PtoSVec = obj_->WT_.position_ - playerObj_->WT_.position_;
		PtoSVec = PtoSVec.normalize();
		obj_->WT_.quaternion_ = DirectionToDirection(Vector3(0, 1, 0), PtoSVec);

		
	}
	else {
		//ƒvƒŒƒCƒ„[‚Ì”w’†‚ÉŒü‚©‚Á‚Ä™X‚ÉˆÚ“®‚·‚é
		Vector3 pos = playerObj_->WT_.position_;
		Vector3 frontVec = {
				sinf(playerObj_->WT_.rotation_.y),
				0,
				cosf(playerObj_->WT_.rotation_.y),
		};
		frontVec = frontVec.normalize();

		goalPos_ = pos - frontVec * 1.2f;
		goalPos_.y += playerObj_->WT_.scale_.y;

		Vector3 nowToGoalVec = goalPos_ - obj_->GetTransform()->position_;

		nowPos_ += nowToGoalVec * 0.1f;

		obj_->SetPos(nowPos_);
		obj_->WT_.quaternion_ = obj_->WT_.quaternion_.Slerp(Quaternion(0,1,0,0),0.1f);
	}

	

	

	

	obj_->Update();
}

void Sword::Draw()
{
	obj_->Draw();
}
