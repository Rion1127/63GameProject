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

	cameraSpeed_ = 0.15f;
}

void GameCamera::Update(CameraMode cameraMode)
{
	UpdateCameraPos();
	if (cameraMode == CameraMode::LookAT)
	{
		UpdateLookAT();
	}
	else if (cameraMode == CameraMode::LookTo)
	{
		UpdateLookTO();
	}

	camera_->eye_ += (endEyePos_ - camera_->eye_) * cameraSpeed_;
	
}

void GameCamera::UpdateCameraPos()
{
	Vector3 cameraTrans = {
		player_->GetWorldTransform()->position_.x,
		player_->GetWorldTransform()->position_.y,
		player_->GetWorldTransform()->position_.z
	};

	float frontdist = 15;

	if (camera_->eye_.y < player_->GetWorldTransform()->position_.y + 30)
	{
		moveDist.x -= controller_->GetRStick(deadZone_.x).x * 0.0000015f;
		moveDist.y += controller_->GetRStick(deadZone_.y).y * 0.0000015f;
		//カメラがどのくらいプレイヤーに近づくかClampをする
		moveDist.y = Clamp(moveDist.y, -0.8f, 1.2f);
	}

	//ロックオンしている敵がいる場合カメラが自動的に画面内に映すように移動する
	if (player_->GetAttackManager()->GetLockOnEnemy())
	{

		endEyePos_.x = -frontdist * sinf(moveDist.x) * cosf(moveDist.y) + cameraTrans.x;
		endEyePos_.y = frontdist * sinf(moveDist.y) + cameraTrans.y;
		endEyePos_.z = -frontdist * cosf(moveDist.x) * cosf(moveDist.y) + cameraTrans.z;
	}
	//ロックオンしている敵がいない時のカメラ
	else
	{
		endEyePos_.x = -frontdist * sinf(moveDist.x) * cosf(moveDist.y) + cameraTrans.x;
		endEyePos_.y = frontdist * sinf(moveDist.y) + cameraTrans.y;
		endEyePos_.z = -frontdist * cosf(moveDist.x) * cosf(moveDist.y) + cameraTrans.z;
	}

	float maxGamecameraY = player_->GetWorldTransform()->position_.y + 25;
	float minGamecameraY = 0.5f;

	camera_->eye_.y = Clamp(camera_->eye_.y, minGamecameraY, maxGamecameraY);

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
	ImGui::SliderFloat("q.x", &camera_->WT_.quaternion_.x, -1.0f, 1.0f, "x = %.3f");
	ImGui::SliderFloat("q.y", &camera_->WT_.quaternion_.y, -1.0f, 1.0f, "y = %.3f");
	ImGui::SliderFloat("q.z", &camera_->WT_.quaternion_.z, -1.0f, 1.0f, "z = %.3f");
	ImGui::SliderFloat("q.w", &camera_->WT_.quaternion_.w, -1.0f, 5.0f, "w = %.3f");

	/*ImGui::SliderFloat("sideVec.x", &sideVec.x, 0.0f, 2000.0f, "x = %.3f");
	ImGui::SliderFloat("sideVec.y", &sideVec.y, 0.0f, 2000.0f, "y = %.3f");*/

	ImGui::End();
}

void GameCamera::UpdateLookAT()
{
	//ロックオンしている敵がいる場合カメラが自動的に画面内に映すように移動する
	if (player_->GetAttackManager()->GetLockOnEnemy())
	{
		IEnemy* enemy = player_->GetAttackManager()->GetLockOnEnemy();
		Vector2 screenPos = GetScreenPos(*enemy->GetWorldTransform(),*Camera::scurrent_);
		//ロックオンした敵が画面外に出そうだったら
		if (GetOutScreenEnemy() == GetOutEnemy::Right)
		{
			moveDist.x += 0.01f;
		}

		if (GetOutScreenEnemy() == GetOutEnemy::Left)
		{
			moveDist.x -= 0.01f;
		}

		if (GetOutScreenEnemy() == GetOutEnemy::Up)
		{
			moveDist.y -= 0.01f;
		}

		if (GetOutScreenEnemy() == GetOutEnemy::Down)
		{
			moveDist.y += 0.01f;
		}

		endTargetPos_ = player_->GetWorldTransform()->position_;
	}
	//ロックオンしている敵がいない時のカメラ
	else
	{
		endTargetPos_ = player_->GetWorldTransform()->position_;
	}

	camera_->target_ += (endTargetPos_ - camera_->target_) * cameraSpeed_;
}

void GameCamera::UpdateLookTO()
{
	//ロックオンしている敵がいる場合カメラが自動的に画面内に映すように移動する
	if (player_->GetAttackManager()->GetLockOnEnemy())
	{
		Vector3 cameraToPlayer =
			player_->GetWorldTransform()->position_ - camera_->eye_;
		cameraToPlayer.normalize();

		Quaternion q;

		q = DirectionToDirection({0,0,1}, cameraToPlayer);
		
		camera_->WT_.SetQuaternion(q);

		endRot_ = {0,0,0};
	}
	//ロックオンしている敵がいない時のカメラ
	else
	{
		Vector3 cameraToPlayer =
			player_->GetWorldTransform()->position_ - camera_->eye_;
		cameraToPlayer.normalize();

		Vector3 angle = {
			acosf(cameraToPlayer.x),
			acosf(cameraToPlayer.y),
			acosf(cameraToPlayer.z),
		};

		endRot_ = angle;
	}

	camera_->rot_ += (endRot_ - camera_->rot_) * cameraSpeed_;
}

GetOutEnemy GameCamera::GetOutScreenEnemy()
{
	IEnemy* enemy = player_->GetAttackManager()->GetLockOnEnemy();
	Vector2 screenPos = GetScreenPos(*enemy->GetWorldTransform(), *Camera::scurrent_);
	Vector2 length = {320,120};
	Vector2 halfWindowSize = WinAPI::GetWindowSize() / 2.f;
	
	if (screenPos.x < halfWindowSize.x - length.x)
	{
		return GetOutEnemy::Left;
	}
	if (screenPos.x > halfWindowSize.x + length.x)
	{
		return GetOutEnemy::Right;
	}

	if (screenPos.y < halfWindowSize.y - length.y)
	{
		return GetOutEnemy::Up;
	}
	if (screenPos.y > halfWindowSize.y + length.y)
	{
		return GetOutEnemy::Down;
	}
	return GetOutEnemy::Middle;
}
