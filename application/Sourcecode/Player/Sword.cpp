#include "Sword.h"
#include "util.h"
#include <imgui.h>
#include "ParticleHitAttack.h"
#include <imgui.h>
#include "myMath.h"

Sword::Sword()
{
	obj_ = MakeUnique<Object3d>();
	obj_->SetModel(Model::CreateOBJ_uniptr("sword"));
	obj_->SetScale({ 0.5f,0.5f, 0.5f });
	obj_->WT_.SetRotType(RotType::Quaternion);

	floatingTimer_.SetLimitTime(120);
	floatingTimer_.SetIsLoop(true);
	
	trail_ = std::make_unique<SwordTrail>(10);

	tailObj_.resize(2);
	for (uint32_t i = 0; i < tailObj_.size(); i++) {
		tailObj_[i] = std::make_unique<Object3d>();
		tailObj_[i]->SetModel(Model::CreateOBJ_uniptr("sphere"));
		tailObj_[i]->SetScale({ 0.5f,0.5f, 0.5f });
		tailObj_[i]->WT_.parent_ = &obj_->WT_;
		tailObj_[i]->SetIsVisible(false);
	}
}

void Sword::Update()
{
	trail_->SetIsVisible(false);
	//UŒ‚Žž‚ÌŒ•‚Ì“®‚«
	if (state_ == SwordState::Idle) {
		//ƒvƒŒƒCƒ„[‚Ì”w’†‚ÉŒü‚©‚Á‚Ä™X‚ÉˆÚ“®‚·‚é
		Vector3 pos = playerObj_->WT_.position_;
		Vector3 frontVec = {
				sinf(playerObj_->WT_.rotation_.y),
				0,
				cosf(playerObj_->WT_.rotation_.y),
		};
		frontVec = frontVec.normalize();
		//À•WˆÚ“®
		goalPos_ = pos - frontVec * 1.2f;
		goalPos_.y += playerObj_->WT_.scale_.y;
		//ã‰º‚É•‚‚©‚Î‚¹‚é
		floatingTimer_.AddTime(1);
		float roundTime = (float)floatingTimer_.GetLimitTimer();
		float timer = (float)floatingTimer_.GetTimer();
		float floatingPos = UpAndDown(roundTime,0.3f, timer);
		goalPos_.y += floatingPos;

		Vector3 nowToGoalVec = goalPos_ - obj_->GetTransform()->position_;
		nowPos_ += nowToGoalVec * 0.1f;
		obj_->SetPos(nowPos_);
		//‰ñ“]ˆ—
		//í‚É‰ñ“]‚³‚¹‚é
		rot_ += 0.02f;
		if (rot_ >= 3.14f) {
			rot_ = -rot_;
		}

		Vector3 axisY = {0, 1, 0};

		obj_->WT_.quaternion_ = obj_->WT_.quaternion_.Slerp(MakeAxisAngle(axisY,rot_), 0.1f);
	}
	else if (state_ == SwordState::Attack &&
		attackManager_->GetNowAttack() != nullptr)
	{
		//‰ñ“]‚ÌeŽqŠÖŒW‚ð‰ðœ
		obj_->WT_.parentRotMat_ = nullptr;
		//À•W
		Vector3 pos = attackManager_->GetNowAttack()->GetSwordPos();
		localPos_ = pos - playerObj_->WT_.position_;
		obj_->SetPos(pos);
		nowPos_ = pos;
		//‰ñ“]î•ñ
		Vector3 PtoSVec = obj_->WT_.position_ - playerObj_->WT_.position_;
		PtoSVec = PtoSVec.normalize();

		obj_->WT_.quaternion_ = DirectionToDirection(Vector3(0, 1, 0), PtoSVec);
		trail_->SetIsVisible(true);
	}
	else if (state_ == SwordState::Guard) {
		//‰ñ“]s—ñ‚ðeŽqŠÖŒW‚É‚·‚é
		obj_->WT_.parentRotMat_ = &playerObj_->WT_.rotMat_;
		//À•W
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
		//‰ñ“]î•ñ
		Quaternion guard = { 2.5f, 1, 0, 0 };
		guard = guard.Normalize();
		obj_->WT_.quaternion_ = obj_->WT_.quaternion_.Slerp(guard, 0.5f);
	}

	if (state_ != SwordState::Idle) {
		rot_ = 0;
	}

	obj_->Update();

	
	CalculateTrailPos();
	trail_->Update();
}

void Sword::Draw()
{
	trail_->Draw();
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	obj_->Draw();

	for (uint32_t i = 0; i < tailObj_.size(); i++) {
		tailObj_[i]->Draw();
	}
#ifdef _DEBUG
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
#endif // _DEBUG
}

void Sword::CalculateTrailPos()
{
	for (uint32_t i = 0; i < tailObj_.size(); i++) {
		//head
		if (i == 0) {
			tailObj_[i]->SetPos(Vector3(0, 3.7f, 0));
		}
		//tail
		else {
			tailObj_[i]->SetPos(Vector3(0, 1.f, 0));
		}
		tailObj_[i]->Update();
	}

	Vector3 head = tailObj_[0]->WT_.GetWorldPos();
	Vector3 tail = tailObj_[1]->WT_.GetWorldPos();
	trail_->SetPos(head, tail);
}
