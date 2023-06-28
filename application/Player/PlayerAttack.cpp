#include <cmath>
#include "Util.h"
#include "PlayerAttack.h"

void PlayerAttack::Init()
{
	controler_ = Controller::GetInstance();
	model_ = std::move(std::make_unique<Object3d>());
	model_->SetModel(Model::CreateOBJ_uniptr("cube", false));

	step = 0;
}

void PlayerAttack::Update(WorldTransform* playerPos)
{
	if (controler_->GetTriggerButtons(PAD_B)) {
		isNext_ = true;
	}

	if (actionTimer_ >= maxActionTimer_) {
		//コンボ中にボタンを押したら次のコンボへ
		if (isNext_ == true) {
			step++;
			startCount = 0;
		}
		//押してない場合は次の攻撃をしない
		else {
			step = 0;

		}
		startIndex = 1;
		timeRate = 0;
		startCount = 0;

		actionTimer_ = 0;
		maxActionTimer_ = 0;
		isNext_ = false;
	}

	Vector3 frontVec = { 0,0,1 };
	Vector3 resultVec;
	Vector3 upVec = { 0,1,0 };
	Vector3 sideVec;

	//プレイヤーの正面ベクトル
	resultVec.x = {
	  cos(playerPos->rotation_.y) * frontVec.x
	  + sin(playerPos->rotation_.y) * frontVec.z
	};
	resultVec.z = {
	  -sin(playerPos->rotation_.y) * frontVec.x +
	  cos(playerPos->rotation_.y) * frontVec.z
	};

	sideVec = upVec.cross(resultVec);
	sideVec.normalize();


	float dist = 7;

	col_.isActive = false;

	if (step == 1) {
		//ステップの最大時間
		maxActionTimer_ = 20;

		col_.isActive = true;

#pragma region スプライン曲線
		//スプライン曲線
		Vector3 point1, point2, point3;
		point1 = playerPos->position_ + -sideVec * dist;
		point2 = playerPos->position_ + resultVec * dist;
		point3 = playerPos->position_ + sideVec * dist;
		attackPoint = { point1 ,point1 ,point2 ,point3,point3 };

#pragma endregion

		actionTimer_++;
	}
	else if (step == 2) {
		maxActionTimer_ = 20;

		col_.isActive = true;

#pragma region スプライン曲線
		//スプライン曲線
		Vector3 point1, point2, point3;
		point1 = playerPos->position_ + sideVec * dist;
		point2 = playerPos->position_ + resultVec * dist;
		point3 = playerPos->position_ + -sideVec * dist;
		attackPoint = { point1 ,point1 ,point2 ,point3,point3 };

#pragma endregion

		actionTimer_++;
	}
	else if (step == 3) {
		maxActionTimer_ = 30;

		col_.isActive = true;

		//スプライン曲線
		Vector3 point1, point2, point3;
		point1 = playerPos->position_ + upVec * dist;
		point2 = playerPos->position_ + resultVec * dist;
		point3 = playerPos->position_ + resultVec * dist;
		attackPoint = { point1 ,point1 ,point2 ,point3,point3 };

		

		actionTimer_++;
	}

	if (step != 0) {
		//スプライン曲線更新
		worldTransform_.position_ = UpdateSpline();
	}
	model_->SetPos(worldTransform_.position_);
	model_->Update();
}

void PlayerAttack::DrawImGui()
{
	ImGui::Begin("Attack");
	/* ここに追加したいGUIを書く */
	// Menu Bar
	if (ImGui::CollapsingHeader("Posision"))
	{
		float x = worldTransform_.position_.x;
		float y = worldTransform_.position_.y;
		float z = worldTransform_.position_.z;
		ImGui::SliderFloat("pos.x", &x, 0.0f, 2000.0f, "x = %.3f");
		ImGui::SliderFloat("pos.y", &y, 0.0f, 2000.0f, "y = %.3f");
		ImGui::SliderFloat("pos.y", &z, 0.0f, 2000.0f, "y = %.3f");

	}

	//回転
	if (ImGui::CollapsingHeader("step"))
	{
		ImGui::SliderInt("step", &step, 0, 10, "x = %d");
		ImGui::SliderInt("maxActionTimer", &maxActionTimer_, 0, maxActionTimer_, "x = %d");
		ImGui::SliderInt("actionTimer_", &actionTimer_, 0, maxActionTimer_, "x = %d");

		/*ImGui::SliderFloat("timeRate", &timeRate, 0.0f, 10, "x = %d");
		ImGui::SliderFloat("startCount", &startCount, 0.0f, startCount, "x = %d");
		ImGui::SliderFloat("nowCount", &nowCount, 0.0f, nowCount, "x = %d");*/
	}

	/*ImGui::SliderFloat("Max X", &MaxMinX.x, 0.f, 80.f, "x = %.3f");
	ImGui::SliderFloat("Min X", &MaxMinX.y, -80.f, 0.f, "y = %.3f");

	ImGui::SliderFloat("Max Y", &MaxMinY.x, 0.f, 80.f, "x = %.3f");
	ImGui::SliderFloat("Min Y", &MaxMinY.y, -80.f, 0.f, "y = %.3f");


	ImGui::SliderFloat("front.x", &resultVec2.x, 0.0f, 2000.0f, "x = %.3f");
	ImGui::SliderFloat("front.y", &resultVec2.y, 0.0f, 2000.0f, "y = %.3f");

	ImGui::SliderFloat("input.x", &inputVec_.x, -10.0f, 10.0f, "x = %.3f");
	ImGui::SliderFloat("input.y", &inputVec_.y, -10.0f, 10.0f, "y = %.3f");*/

	ImGui::End();
}

void PlayerAttack::DrawCol()
{
	model_->Draw();
}

Vector3 PlayerAttack::UpdateSpline()
{
	nowCount += 30;
	// 網(弾)発射してからの経過時間[s]
	float nowTime = (nowCount - startCount) / 1000.0f;

	// 経過時間の割合
	float t = nowTime / (static_cast<float>(3) / maxActionTimer_);

	t = Clamp(t, 1.0f, 0.0f);

	if (t >= 1.0f) {
		//次の制御点がある場合
		if (startIndex < attackPoint.size() - 3) {
			startIndex++;
			timeRate = 0;
			t = 0;
			//経過時間をリセット
			startCount = 0;
			nowCount = 0;
		}
		//最終地点だった場合1.0fにして動きを止める
		else {
			timeRate = 1.0f;
		}
	}

	return SplinePosition(attackPoint, startIndex, t);
}
