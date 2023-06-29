#include <imgui.h>
#include "Player.h"
#include "Util.h"
#include "Camera.h"

Player::Player()
{
	controller_ = Controller::GetInstance();

	colPos_.radius = 1;

	// 入力されているベクトル
	inputVec_ = { 0.0f, 0.0f };
	// 入力されている方向の角度
	inputAngle_ = 0.0f;
	//左スティックが倒されているか
	isLeftStick_ = false;
	//1フレーム前の左スティックが倒されているか
	isOldLeftStick_ = false;

	//移動速度
	moveSpeed_ = 0.3f;

	model_ = std::move(std::make_unique<Object3d>());
	model_->SetModel(Model::CreateOBJ_uniptr("player", true));

	scale_ = { 1,1,1 };
}

void Player::Update()
{

	// 入力方向ベクトルを更新
	InputVecUpdate();
	// 入力方向の角度を更新
	InputAngleUpdate();
	//ジャンプ
	JumpUpdate();

	//重力
	GravityUpdate();

	ColPosUpdate();

	// 回転代入
	rot_ = { 0,Radian(inputAngle_) ,0 };

	model_->SetRot(rot_);
	model_->SetPos(pos_);
	model_->SetScale(scale_);
	model_->Update();
	attack_.Update();
}
void Player::ColPosUpdate()
{
	model_->SetRot(rot_);
	model_->SetPos(pos_);
	model_->SetScale(scale_);
	model_->Update();

	//モデルの原点を下にしているためその分ずらす
	Vector3 colPos = {
		model_->GetTransform()->position_.x,
		model_->GetTransform()->position_.y + 1.0f,
		model_->GetTransform()->position_.z,
	};

	colPos_.SetPos(colPos);

	
}
#pragma region 入力
void Player::InputVecUpdate()
{
	//プレイヤーの正面ベクトル
	resultVec = {
		Camera::scurrent_.target_.x - Camera::scurrent_.eye_.x,
		Camera::scurrent_.target_.y - Camera::scurrent_.eye_.y,
		Camera::scurrent_.target_.z - Camera::scurrent_.eye_.z
	};
	resultVec.normalize();
	resultVec2 = {
		resultVec.x,
		resultVec.z,
	};

	sideVec = upVec.cross(resultVec);
	sideVec.normalize();

	// コントローラーが接続されていたら
	if (controller_->GetActive())
	{
		// 左スティックの入力方向ベクトル取得
		inputVec_ = controller_->GetLStick() / 3276.8f;
		//カメラから見た左右手前奥移動
		moveVec2.x = (resultVec2.y * -inputVec_.x) + (sideVec.z * inputVec_.y);
		moveVec2.y = (resultVec2.y * inputVec_.y) + (sideVec.z * inputVec_.x);
		moveVec2.x = -moveVec2.x;
		moveVec2 *= 0.03f;

		// 1フレーム前の情報として保存
		isOldLeftStick_ = isLeftStick_;

		// 左スティックが倒されていたら
		if (inputVec_.length() >= 0.5f) isLeftStick_ = true;
		else isLeftStick_ = false;
	}

	pos_ += {moveVec2.x, 0, moveVec2.y};

	pos_ = {
		Clamp(pos_.x, -77.f, 77.f),
		Clamp(pos_.y, 0.f, 100.f),
		Clamp(pos_.z, -77.f, 77.f)
	};

	// 入力しているベクトルの角度を求める
	float inputAngle = Vec2Angle(moveVec2);

	// 計算結果がオーバーフローしていなかったら値を更新
	if (inputAngle >= 0)
	{
		inputAngle_ = inputAngle;
	}
}

void Player::InputAngleUpdate()
{

}

void Player::GravityUpdate()
{
	float gravitySpeed = -0.01f;
	//床に触れていたら
	if (isFloorCollision)
	{
		gravity_ = 0;
	}
	//空中にいたら重力を足す
	else
	{
		gravity_ += gravitySpeed;
	}

	//model_->GetTransform()->AddPosition(0, gravity_,0);
	//通常時はfalseにしておく
	isFloorCollision = false;
}

void Player::JumpUpdate()
{
	float jumpSpeed = 0.2f;
	int Maxjumptimer = 20;
	if (controller_->GetButtons(PAD::INPUT_A))
	{
		if (isJump_ == false)
		{
			if (jumpTime < Maxjumptimer)
			{
				jumpTime++;
				//重力をマイナスにする
				gravity_ = jumpSpeed;
			}
		}
	}
	//押し戻し処理がまだ

	if (controller_->GetReleasButtons(PAD::INPUT_A))
	{
		isJump_ = true;
	}
	pos_ += { 0, gravity_, 0 };
}

void Player::Draw()
{
	model_->Draw();

	DrawImGui();

	attack_.DrawDebug();
}

void Player::DrawImGui()
{
	ImGui::Begin("Player");
	/* ここに追加したいGUIを書く */
	// Menu Bar
	if (ImGui::CollapsingHeader("Posision"))
	{
		float x = model_->GetTransform()->position_.x;
		float y = model_->GetTransform()->position_.y;
		float z = model_->GetTransform()->position_.z;
		ImGui::SliderFloat("pos.x", &x, 0.0f, 2000.0f, "x = %.3f");
		ImGui::SliderFloat("pos.y", &y, 0.0f, 2000.0f, "y = %.3f");
		ImGui::SliderFloat("pos.y", &z, 0.0f, 2000.0f, "y = %.3f");

	}

	//回転
	if (ImGui::CollapsingHeader("Rotation"))
	{
		float rot = model_->GetTransform()->rotation_.y;
		ImGui::SliderFloat("Rot", &rot, 0.0f, Radian(360), "x = %.3f");
		//rot_ = rot;
	}

	if (ImGui::CollapsingHeader("Scale"))
	{
		float x = model_->GetTransform()->scale_.x;
		float y = model_->GetTransform()->scale_.y;
		float z = model_->GetTransform()->scale_.z;
		ImGui::SliderFloat("scale.x", &x, 0.0f, 2000.0f, "x = %.3f");
		ImGui::SliderFloat("scale.y", &y, 0.0f, 2000.0f, "y = %.3f");
		ImGui::SliderFloat("scale.y", &z, 0.0f, 2000.0f, "y = %.3f");

	}

	ImGui::SliderFloat("Max X", &MaxMinX.x, 0.f, 80.f, "x = %.3f");
	ImGui::SliderFloat("Min X", &MaxMinX.y, -80.f, 0.f, "y = %.3f");

	ImGui::SliderFloat("Max Y", &MaxMinY.x, 0.f, 80.f, "x = %.3f");
	ImGui::SliderFloat("Min Y", &MaxMinY.y, -80.f, 0.f, "y = %.3f");


	ImGui::SliderFloat("front.x", &resultVec2.x, 0.0f, 2000.0f, "x = %.3f");
	ImGui::SliderFloat("front.y", &resultVec2.y, 0.0f, 2000.0f, "y = %.3f");

	ImGui::SliderFloat("input.x", &inputVec_.x, -10.0f, 10.0f, "x = %.3f");
	ImGui::SliderFloat("input.y", &inputVec_.y, -10.0f, 10.0f, "y = %.3f");

	ImGui::End();

}

void Player::floorColision()
{
	isFloorCollision = true;
	isJump_ = false;
	jumpTime = 0;
}
