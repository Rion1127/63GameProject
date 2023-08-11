#include "Sword.h"
#include "util.h"
#include <imgui.h>
#include "ParticleHitAttack.h"
#include <imgui.h>

Sword::Sword()
{
	obj_ = MakeUnique<Object3d>();
	obj_->SetModel(Model::CreateOBJ_uniptr("sword"));
	obj_->SetScale({ 0.5f,0.5f, 0.5f });
	obj_->WT_.SetRotType(RotType::Quaternion);
}

void Sword::Update()
{
	//obj_->WT_.parentRotMat_ = &playerObj_->WT_.rotMat_;
	//攻撃時の剣の動き
	if (state_ == SwordState::Idle) {
		//プレイヤーの背中に向かって徐々に移動する
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
		obj_->WT_.quaternion_ = obj_->WT_.quaternion_.Slerp(Quaternion(0, 1, 0, 0), 0.1f);
	}
	else if (state_ == SwordState::Attack &&
		attackManager_->GetNowAttack() != nullptr)
	{
		//座標
		Vector3 pos = attackManager_->GetNowAttack()->GetAttackCol()->at(0)->col_.center;
		localPos_ = pos - playerObj_->WT_.position_;
		obj_->SetPos(pos);
		nowPos_ = pos;
		//回転情報
		Vector3 PtoSVec = obj_->WT_.position_ - playerObj_->WT_.position_;
		PtoSVec = PtoSVec.normalize();

		obj_->WT_.quaternion_ = DirectionToDirection(Vector3(0, 1, 0), PtoSVec);
	}
	else if (state_ == SwordState::Guard) {
		//座標
		Vector3 pos = playerObj_->WT_.position_;
		Vector3 frontVec = {
				sinf(playerObj_->WT_.rotation_.y),
				0,
				cosf(playerObj_->WT_.rotation_.y),
		};
		frontVec = frontVec.normalize();

		goalPos_ = pos + frontVec * 1.1f;
		goalPos_.y += playerObj_->WT_.scale_.y * 1.5f;

		Vector3 nowToGoalVec = goalPos_ - obj_->GetTransform()->position_;

		nowPos_ += nowToGoalVec * 0.5f;

		obj_->SetPos(nowPos_);
		//回転情報
		Quaternion guard = { 2.5f, 1, 0, 0 };
		guard = guard.Normalize();
		obj_->WT_.quaternion_ = obj_->WT_.quaternion_.Slerp(guard, 0.5f);
	}

	obj_->Update();
}

void Sword::Draw()
{
	obj_->Draw();

	ImGui::Begin("sword");

	if (ImGui::Button("debugMode")) {
		if (state_ != SwordState::Debug) {
			state_ = SwordState::Debug;
		}
		else {
			state_ = SwordState::Idle;
		}
	}

	float value[4] = {
		obj_->WT_.quaternion_.x,
		obj_->WT_.quaternion_.y,
		obj_->WT_.quaternion_.z,
		obj_->WT_.quaternion_.w,
	};
	ImGui::DragFloat4("quaternion", value,0.1f);

	obj_->WT_.quaternion_.x = value[0];
	obj_->WT_.quaternion_.y = value[1];
	obj_->WT_.quaternion_.z = value[2];
	obj_->WT_.quaternion_.w = value[3];

	float pos[3] = {
		obj_->WT_.position_.x,
		obj_->WT_.position_.y,
		obj_->WT_.position_.z,
	};
	ImGui::DragFloat3("pos", pos, 0.1f);

	obj_->WT_.position_.x = pos[0];
	obj_->WT_.position_.y = pos[1];
	obj_->WT_.position_.z = pos[2];

	ImGui::End();

}
