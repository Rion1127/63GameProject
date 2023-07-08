#include "GameCamera.h"
#include <imgui.h>

GameCamera::GameCamera()
{
	camera_ = std::move(std::make_unique<Camera>());
	controller_ = Controller::GetInstance();

	deadZone_ = {
		DeadZone::Mul_2,
		DeadZone::Default
	};
}

void GameCamera::Update()
{
	Vector3 cameraTrans = {
		player_->GetWorldTransform()->position_.x,
		player_->GetWorldTransform()->position_.y,
		player_->GetWorldTransform()->position_.z
	};

	float frontdist = 15;

	static Vector3 moveDist{};	//球面座標

	if (camera_->eye_.y < player_->GetWorldTransform()->position_.y + 30)
	{
		moveDist.x -= controller_->GetRStick(deadZone_.x).x * 0.0000015f;
		moveDist.y += controller_->GetRStick(deadZone_.y).y * 0.0000015f;
		//カメラがどのくらいプレイヤーに近づくかClampをする
		moveDist.y = Clamp(moveDist.y, -0.8f, 1.2f);
	}

	
	//ロックオンしている敵がいる場合カメラが自動的に画面内に映すように移動する
	if (player_->GetAttackManager()->GetLockOnEnemy()) {
		IEnemy* enemy = player_->GetAttackManager()->GetLockOnEnemy();
		Vector3 ptoEVec = enemy->GetWorldTransform()->position_ - player_->GetWorldTransform()->position_;
		ptoEVec /= 2.f;

		endEyePos_.x = -frontdist * sinf(moveDist.x) * cosf(moveDist.y) + cameraTrans.x;
		endEyePos_.y = frontdist * sinf(moveDist.y) + cameraTrans.y;
		endEyePos_.z = -frontdist * cosf(moveDist.x) * cosf(moveDist.y) + cameraTrans.z;

		//endTargetPos_ = enemy->GetWorldTransform()->position_ + ptoEVec;
		endTargetPos_ = player_->GetWorldTransform()->position_;
	}
	//ロックオンしている敵がいない時のカメラ
	else {
		
		endEyePos_.x = -frontdist * sinf(moveDist.x) * cosf(moveDist.y) + cameraTrans.x;
		endEyePos_.y = frontdist * sinf(moveDist.y) + cameraTrans.y;
		endEyePos_.z = -frontdist * cosf(moveDist.x) * cosf(moveDist.y) + cameraTrans.z;

		endTargetPos_ = player_->GetWorldTransform()->position_;
	}

	camera_->eye_ += (endEyePos_ - camera_->eye_) * 0.2f;
	camera_->target_ += (endTargetPos_ - camera_->target_) * 0.2f;
	
	float maxGamecameraY = player_->GetWorldTransform()->position_.y + 25;
	float minGamecameraY = 0.5f;

	
	camera_->eye_.y = Clamp(camera_->eye_.y, minGamecameraY,maxGamecameraY);

	ImGui::Begin("GameCamera");
	/* ここに追加したいGUIを書く */
	// Menu Bar
	if (ImGui::CollapsingHeader("Posision"))
	{
		static float x = camera_->eye_.x;
		static float y = camera_->eye_.y;
		static float z = camera_->eye_.z;
		ImGui::SliderFloat("pos.x", &camera_->eye_.x, 0.0f, 2000.0f, "x = %.3f");
		ImGui::SliderFloat("pos.y", &camera_->eye_.y, 0.0f, 2000.0f, "y = %.3f");
		ImGui::SliderFloat("pos.z", &camera_->eye_.z, 0.0f, 2000.0f, "z = %.3f");
		//viewProjection.eye.x = x;
		//viewProjection.eye.y = y;
		//viewProjection.eye.z = z;
	}

	ImGui::SliderFloat("moveDist.x", &moveDist.x, 0.0f, 2000.0f, "x = %.3f");
	ImGui::SliderFloat("moveDist.y", &moveDist.y, 0.0f, 2000.0f, "y = %.3f");

	/*ImGui::SliderFloat("sideVec.x", &sideVec.x, 0.0f, 2000.0f, "x = %.3f");
	ImGui::SliderFloat("sideVec.y", &sideVec.y, 0.0f, 2000.0f, "y = %.3f");*/

	ImGui::End();
}