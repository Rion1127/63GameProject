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
	trail_->SetColor(Color(255, 175, 60, 255));
}

void Sword::Update()
{
	trail_->SetIsVisible(false);
	//攻撃時の剣の動き
	if (state_ == SwordState::Idle) {
		//プレイヤーの背中に向かって徐々に移動する
		Vector3 pos = playerObj_->WT_.position_;
		Vector3 frontVec = RotateVector(Vector3(0, 0, 1), playerObj_->WT_.quaternion_);
		frontVec = frontVec.normalize();
		//座標移動
		goalPos_ = pos - frontVec * 1.2f;
		//上下に浮かばせる
		floatingTimer_.AddTime(1 * GameSpeed::GetGameSpeed());
		float roundTime = (float)floatingTimer_.GetLimitTimer();
		float timer = (float)floatingTimer_.GetTimer();
		float floatingPos = UpAndDown(roundTime, 0.3f, timer);
		goalPos_.y += floatingPos;

		Vector3 nowToGoalVec = goalPos_ - obj_->GetTransform()->position_;
		nowPos_ += (nowToGoalVec * 0.1f) * GameSpeed::GetGameSpeed();
		obj_->SetPos(nowPos_);
		//回転処理
		//常に回転させる
		rot_ += 0.02f * GameSpeed::GetGameSpeed();
		if (rot_ >= 3.14f) {
			rot_ = -rot_;
		}

		Vector3 axisY = { 0, 1, 0 };

		obj_->WT_.quaternion_ = obj_->WT_.quaternion_.Slerp(MakeAxisAngle(axisY, rot_), 0.1f);
	}
	else if (state_ == SwordState::Attack &&
		attackManager_->GetNowAttack() != nullptr)
	{
		//回転の親子関係を解除
		obj_->WT_.parentRotMat_ = nullptr;
		//座標
		Vector3 pos = attackManager_->GetNowAttack()->GetSwordPos();
		localPos_ = pos - playerObj_->WT_.position_;
		obj_->SetPos(pos);
		nowPos_ = pos;
		//回転情報
		Vector3 PtoSVec = obj_->WT_.position_ - playerObj_->WT_.position_;
		PtoSVec = PtoSVec.normalize();

		obj_->WT_.quaternion_ = DirectionToDirection(Vector3(0, 1, 0), PtoSVec);
		trail_->SetIsVisible(true);

		CalculateTrailPos();
	}
	else if (state_ == SwordState::Guard) {
		//回転行列を親子関係にする
		obj_->WT_.parentRotMat_ = &playerObj_->WT_.rotMat_;
		//座標
		Vector3 pos = playerObj_->WT_.position_;
		Vector3 frontVec = RotateVector(Vector3(0,0,1), playerObj_->WT_.quaternion_);
		frontVec = frontVec.normalize();

		goalPos_ = pos + frontVec * 1.1f;
		
		Vector3 nowToGoalVec = goalPos_ - obj_->GetTransform()->position_;

		nowPos_ += (nowToGoalVec * 0.5f) * GameSpeed::GetGameSpeed();

		obj_->SetPos(nowPos_);
		//回転情報
		Quaternion guard = { 2.5f, 1, 0, 0 };
		guard = guard.Normalize();
		obj_->WT_.quaternion_ = obj_->WT_.quaternion_.Slerp(guard, 0.5f);
	}

	if (state_ != SwordState::Idle) {
		rot_ = 0;
	}

	obj_->Update();



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
	ImGui::DragFloat4("quaternion", value, 0.1f);

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

	static float color[3] = {
		trail_->GetColor().r,
		trail_->GetColor().g,
		trail_->GetColor().b
	};
	//ImGui::DragFloat4("color", color, 0.1f);

	ImGui::ColorEdit3("color 1", color);

	Color col = { color[0],color[1], color[2], 255 };
	trail_->SetColor(col);

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
