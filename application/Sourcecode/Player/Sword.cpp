#include "Sword.h"
#include "util.h"
#include <imgui.h>
#include "ParticleHitAttack.h"
#include <imgui.h>
#include "myMath.h"

/**
 * @file Sword.cpp
 * @brief 剣の動きを管理するクラス
 */

Sword::Sword()
{
	obj_ = MakeUnique<Object3d>();
	obj_->SetModel(Model::CreateOBJ_uniptr("sword"));
	obj_->SetScale({ 0.5f,0.5f, 0.5f });
	obj_->WT_.SetRotType(RotType::Quaternion);
	obj_->SetShadowOffsetPos(Vector3(0, -0.7f, 0));
	obj_->SetShadowAtten(Vector3(1, 1.5f, 0));
	obj_->SetShadowFactorAngle(Vector2(0.05f, 0.25f));

	floatingTimer_.SetLimitTime(120);
	floatingTimer_.SetIsLoop(true);

	trail_ = std::make_unique<SwordTrail>(10);

	tailObj_.resize(2);
	for (uint32_t i = 0; i < tailObj_.size(); i++) {
		tailObj_[i] = std::make_unique<Object3d>();
		tailObj_[i]->SetModel(Model::CreateOBJ_uniptr("sphere",false,false));
		tailObj_[i]->SetScale({ 0.5f,0.5f, 0.5f });
		tailObj_[i]->WT_.parent_ = &obj_->WT_;
		tailObj_[i]->SetIsVisible(false);
	}
	trail_->SetColor(Color(255, 175, 60, 255));
	distaice_ = 1.2f;
	floatRange_ = 0.3f;
	rotSpeed_ = 0.02f;

	trailHeadPos_ = Vector3(0, 3.7f, 0);
	trailtailPos_ = Vector3(0, 1.f, 0);
}

void Sword::Update(const Vector3& swordPos)
{
	trail_->SetIsVisible(false);
	//攻撃時の剣の動き
	if (state_ == SwordState::Idle) {
		//回転行列を親子関係にする
		obj_->WT_.parentRotMat_ = &playerObj_->WT_.rotMat_;
		//回転の親子関係を解除
		obj_->WT_.parent_ = nullptr;
		//プレイヤーの背中に向かって徐々に移動する
		Vector3 pos = playerObj_->WT_.position_;
		Vector3 frontVec = RotateVector(Vector3(0, 0, 1), playerObj_->WT_.quaternion_);
		frontVec = frontVec.normalize();
		//座標移動
		goalPos_ = pos - frontVec * distaice_;
		//上下に浮かばせる
		floatingTimer_.AddTime(GameSpeed::GetGameSpeed());
		float roundTime = floatingTimer_.GetLimitTimer();
		float timer = floatingTimer_.GetTimer();
		float floatingPos = UpAndDown(roundTime, floatRange_, timer);
		goalPos_.y += floatingPos;

		Vector3 nowToGoalVec = goalPos_ - obj_->GetTransform()->position_;
		nowPos_ += (nowToGoalVec * 0.1f) * GameSpeed::GetGameSpeed();
		obj_->SetPos(nowPos_);
		//回転処理
		//常に回転させる
		rot_ += rotSpeed_ * GameSpeed::GetGameSpeed();
		if (rot_ >= PI) {
			rot_ = -rot_;
		}

		Vector3 axisY = { 0, 1, 0 };

		obj_->WT_.quaternion_ = obj_->WT_.quaternion_.Slerp(MakeAxisAngle(axisY, rot_), 1.0f);
	}
	else if (state_ == SwordState::Attack)
	{
		//回転の親子関係を解除
		obj_->WT_.parentRotMat_ = nullptr;
		//座標
		Vector3 pos = swordPos;
		localPos_ = pos ;
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
		//回転の親子関係を解除
		obj_->WT_.parent_ = nullptr;
		//座標
		Vector3 pos = playerObj_->WT_.position_;
		Vector3 frontVec = RotateVector(Vector3(0,0,1), playerObj_->WT_.quaternion_);
		frontVec = frontVec.normalize();

		goalPos_ = pos + frontVec * distaice_;
		
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

void Sword::EditorUpdate(const Vector3& swordPos)
{
	trail_->SetIsVisible(false);

	//攻撃時の剣の動き
	if (state_ == SwordState::Idle) {
		//回転行列を親子関係にする
		obj_->WT_.parentRotMat_ = &playerObj_->WT_.rotMat_;
		//回転の親子関係を解除
		obj_->WT_.parent_ = nullptr;
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
	else if (state_ == SwordState::Attack)
	{
		//回転の親子関係を解除
		obj_->WT_.parentRotMat_ = nullptr;
		//座標
		Vector3 pos = swordPos;
		localPos_ = pos;
		obj_->SetPos(pos);
		nowPos_ = pos;
		//回転情報
		Vector3 PtoSVec = obj_->WT_.position_ - playerObj_->WT_.position_;
		PtoSVec = PtoSVec.normalize();

		obj_->WT_.quaternion_ = DirectionToDirection(Vector3(0, 1, 0), PtoSVec);
		trail_->SetIsVisible(true);

		CalculateTrailPos();
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

	
}

void Sword::DrawImGui()
{
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
	
	ImGui::ColorEdit3("color 1", color);

	Color col = { color[0],color[1], color[2], 255 };
	trail_->SetColor(col);

	ImGui::End();
}

void Sword::Reset()
{
	//プレイヤーの背中に向かって徐々に移動する
	Vector3 pos = playerObj_->WT_.position_;
	Vector3 frontVec = RotateVector(Vector3(0, 0, 1), playerObj_->WT_.quaternion_);
	frontVec = frontVec.normalize();
	//座標移動
	goalPos_ = pos - frontVec * distaice_;
	nowPos_ = goalPos_;
}

void Sword::CalculateTrailPos()
{
	for (uint32_t i = 0; i < tailObj_.size(); i++) {
		//head
		if (i == 0) {
			tailObj_[i]->SetPos(trailHeadPos_);
		}
		//tail
		else {
			tailObj_[i]->SetPos(trailtailPos_);
		}
		tailObj_[i]->Update();
	}

	Vector3 head = tailObj_[0]->WT_.GetWorldPos();
	Vector3 tail = tailObj_[1]->WT_.GetWorldPos();
	trail_->SetPos(head, tail);
}
