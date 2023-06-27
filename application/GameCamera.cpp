#include "GameCamera.h"
#include <imgui.h>

GameCamera::GameCamera()
{
	camera_ = std::move(std::make_unique<Camera>());
	controller_ = Controller::GetInstance();
}

void GameCamera::CUpdate()
{
	Vector3 cameraTrans = {
		player_->GetWorldTransform().position_.x,
		player_->GetWorldTransform().position_.y + 20,
		player_->GetWorldTransform().position_.z
	};

	float frontdist = 25;

	static Vector3 moveDist{};	//‹…–ÊÀ•W

	camera_->eye_.x = player_->GetWorldTransform().position_.x;
	camera_->eye_.y = player_->GetWorldTransform().position_.y + 10;
	camera_->eye_.z = player_->GetWorldTransform().position_.z - 25;

	camera_->target_.x = player_->GetWorldTransform().position_.x;
	camera_->target_.y = player_->GetWorldTransform().position_.y;
	camera_->target_.z = player_->GetWorldTransform().position_.z;

	if (camera_->eye_.y < player_->GetWorldTransform().position_.y + 30)
	{
		moveDist.x -= controller_->GetRStick().x * 0.0000015f;
		moveDist.y += controller_->GetRStick().y * 0.0000015f;
		moveDist.y = Clamp(moveDist.y, -1.3f, 1.3f);
	}
	/*moveDist.y += mInput_->GetCursorMoveY() * 0.005f;
	moveDist.z -= mInput_->GetCursorMoveZ() * 0.005f;*/

	camera_->eye_.x = -frontdist * sinf(moveDist.x) * cosf(moveDist.y) + cameraTrans.x;
	camera_->eye_.y = frontdist * sinf(moveDist.y) + cameraTrans.y;
	camera_->eye_.z = -frontdist * cosf(moveDist.x) * cosf(moveDist.y) + cameraTrans.z;

	float maxGamecameraY = player_->GetWorldTransform().position_.y + 25;
	float minGamecameraY = 0.5f;

	
	camera_->eye_.y = Clamp(camera_->eye_.y, minGamecameraY,maxGamecameraY);

	ImGui::Begin("GameCamera");
	/* ‚±‚±‚É’Ç‰Á‚µ‚½‚¢GUI‚ð‘‚­ */
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