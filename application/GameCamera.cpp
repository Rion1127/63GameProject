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

	transSpeed_ = { 0.0000015f,0.0000015f };
	lockOnCameraspeed_ = { 0.04f ,0.01f };
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
		moveDist.x -= controller_->GetRStick(deadZone_.x).x * transSpeed_.x;
		moveDist.y += controller_->GetRStick(deadZone_.y).y * transSpeed_.y;
		//�J�������ǂ̂��炢�v���C���[�ɋ߂Â���Clamp������
		moveDist.y = Clamp(moveDist.y, -0.8f, 1.2f);
	}
	//���ʍ��W���
	endEyePos_.x = -frontdist * sinf(moveDist.x) * cosf(moveDist.y) + cameraTrans.x;
	endEyePos_.y = frontdist * sinf(moveDist.y) + cameraTrans.y;
	endEyePos_.z = -frontdist * cosf(moveDist.x) * cosf(moveDist.y) + cameraTrans.z;

	float maxGamecameraY = player_->GetWorldTransform()->position_.y + 25;
	float minGamecameraY = 0.5f;

	camera_->eye_.y = Clamp(camera_->eye_.y, minGamecameraY, maxGamecameraY);

	ImGui::Begin("GameCamera");
	/* �����ɒǉ�������GUI������ */
	// Menu Bar
	if (ImGui::CollapsingHeader("Posision"))
	{
		static float x = camera_->eye_.x;
		static float y = camera_->eye_.y;
		static float z = camera_->eye_.z;
		ImGui::SliderFloat("pos.x", &camera_->eye_.x, 0.0f, 2000.0f, "x = %.3f");
		ImGui::SliderFloat("pos.y", &camera_->eye_.y, 0.0f, 2000.0f, "y = %.3f");
		ImGui::SliderFloat("pos.z", &camera_->eye_.z, 0.0f, 2000.0f, "z = %.3f");
	}

	ImGui::SliderFloat("moveDist.x", &moveDist.x, 0.0f, 2000.0f, "x = %.3f");
	ImGui::SliderFloat("moveDist.y", &moveDist.y, 0.0f, 2000.0f, "y = %.3f");
	ImGui::SliderFloat("q.x", &camera_->WT_.quaternion_.x, -1.0f, 1.0f, "x = %.3f");
	ImGui::SliderFloat("q.y", &camera_->WT_.quaternion_.y, -1.0f, 1.0f, "y = %.3f");
	ImGui::SliderFloat("q.z", &camera_->WT_.quaternion_.z, -1.0f, 1.0f, "z = %.3f");
	ImGui::SliderFloat("q.w", &camera_->WT_.quaternion_.w, -1.0f, 5.0f, "w = %.3f");

	ImGui::End();
}

void GameCamera::UpdateLookAT()
{
	IEnemy* enemy = player_->GetAttackManager()->GetLockOnEnemy();

	if (enemy != nullptr)
	{
		bool isHardLockOn = enemy->GetIsHardLockOn();
		//���b�N�I�����Ă���G������ꍇ�J�����������I�ɉ�ʓ��ɉf���悤�Ɉړ�����
		if (isHardLockOn)
		{
			//�G�̃X�N���[����̍��W
			Vector2 screenPos = GetScreenPos(*enemy->GetWorldTransform(), *Camera::scurrent_);
			Vector2 windowSize = WinAPI::GetWindowSize();
			Vector2 halfWindowSize = windowSize / 2.f;

			Vector2 length = { 250,300 };
			Vector2 EtoMidvec = screenPos - halfWindowSize;
			EtoMidvec = EtoMidvec.normalize();
			Vector2 addVec;

			if (putOnCamera_ == false)
			{
				//�G����ʂ̂ǂ̈ʒu�ɂ��邩�̔���
				getOutWay = GetOutScreenEnemy(screenPos, windowSize,enemy);
			}
			else
			{
				if (screenPos.x > halfWindowSize.x - length.x &&
					screenPos.x < halfWindowSize.x + length.x &&
					screenPos.y > halfWindowSize.y - length.y &&
					screenPos.y < halfWindowSize.y + length.y)
				{
					putOnCamera_ = false;
				}
			}
			//���b�N�I�������G����ʊO�ɏo������������
			if (getOutWay == GetOutEnemy::Right ||
				getOutWay == GetOutEnemy::Left)
			{
				addVec.x = EtoMidvec.x * lockOnCameraspeed_.x;
			}

			if (getOutWay == GetOutEnemy::Up ||
				getOutWay == GetOutEnemy::Down)
			{
				addVec.y = EtoMidvec.y * lockOnCameraspeed_.y;
			}

			if (getOutWay == GetOutEnemy::LookBack)
			{
				addVec.x = EtoMidvec.x * 0.1f;
			}

			moveDist += addVec;
		}
		//���b�N�I�����Ă���G�����Ȃ����̃J����
		else
		{

		}
	}

	endTargetPos_ = player_->GetWorldTransform()->position_;

	camera_->target_ += (endTargetPos_ - camera_->target_) * cameraSpeed_;
}

void GameCamera::UpdateLookTO()
{
	//���b�N�I�����Ă���G������ꍇ�J�����������I�ɉ�ʓ��ɉf���悤�Ɉړ�����
	if (player_->GetAttackManager()->GetLockOnEnemy())
	{
		Vector3 cameraToPlayer =
			player_->GetWorldTransform()->position_ - camera_->eye_;
		cameraToPlayer = cameraToPlayer.normalize();

		Quaternion q;

		q = DirectionToDirection({ 0,0,1 }, cameraToPlayer);

		camera_->WT_.SetQuaternion(q);
	}
	//���b�N�I�����Ă���G�����Ȃ����̃J����
	else
	{
		Vector3 cameraToPlayer =
			player_->GetWorldTransform()->position_ - camera_->eye_;
		cameraToPlayer = cameraToPlayer.normalize();
	}

	camera_->rot_ += (endRot_ - camera_->rot_) * cameraSpeed_;
}

GetOutEnemy GameCamera::GetOutScreenEnemy(const Vector2& screenPos, const Vector2& winSize, IEnemy* enemy)
{
	Vector2 length = { 550,400 };
	Vector2 halfWindowSize = winSize / 2.f;

	Vector3 PToCDir = player_->GetWorldTransform()->position_ - camera_->eye_;
	Vector3 EToCDir = enemy->GetWorldTransform()->position_ - camera_->eye_;
	//�v���C���[->�J�����̋���
	float PToCLength = PToCDir.length();
	//�G->�J�����̋���
	float EToCLength = EToCDir.length();

	putOnCamera_ = true;
	//���b�N�I�����Ă���G���G���J�����̌��ɍs�������ɂȂ�����U��Ԃ�
	if (PToCLength > EToCLength)
	{
		if (screenPos.x < halfWindowSize.x - length.x ||
			screenPos.x > halfWindowSize.x + length.x ||
			screenPos.y < halfWindowSize.y - length.y ||
			screenPos.y > halfWindowSize.y + length.y)
		{
			return GetOutEnemy::LookBack;
		}
	}

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
	//�J�����������ړ�������t���O���I�t�ɂ���
	putOnCamera_ = false;
	return GetOutEnemy::Middle;
}
