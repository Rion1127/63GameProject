#include "GameCamera.h"
#include <imgui.h>
#include "Easing.h"
#include "RRandom.h"
#include "ConfigMenu.h"

GameCamera::GameCamera()
{
	camera_ = std::move(std::make_unique<Camera>());

	deadZone_ = {
		DeadZone::Mul_2,
		DeadZone::Default
	};

	cameraSpeed_ = 0.15f;

	transSpeed_ = { 0.0000015f,0.0000015f };
	lockOnCameraspeed_ = { 0.04f ,0.01f };

	frontDist_ = 15.f;

	gameCameraMode_ = GameCameraMode::NORMAL;

	Reset();
}

void GameCamera::Update(CameraMode cameraMode)
{

	if (gameCameraMode_ == GameCameraMode::NORMAL) {
		UpdateCameraPos();

		if (clsumSystem_->GetIsClearCamera()) {
			gameCameraMode_ = GameCameraMode::CLEAR;

			frontDist_ = RRandom::RandF(15.f, 20.f);
			randOffsetPos_ = {
				RRandom::RandF(-3.f, 3.f),
				RRandom::RandF(-1.f, 4.f),
				0
			};
		}
	}
	else {
		if (clsumSystem_->GetIsClearCamera() == false) {
			gameCameraMode_ = GameCameraMode::NORMAL;
		}
	}

	if (cameraMode == CameraMode::LookAT)
	{
		UpdateLookAT();
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

	bool isCameraInvX = ConfigMenu::GetInstance()->GetInvX();
	bool isCameraInvY = ConfigMenu::GetInstance()->GetInvY();

	int32_t cameraInvX = (isCameraInvX == false) ? 1 : -1;
	int32_t cameraInvY = (isCameraInvY == false) ? 1 : -1;

	Vector2 inputVec = {
		Controller::GetRStick(deadZone_.x).x * cameraInvX,
		Controller::GetRStick(deadZone_.y).y * cameraInvY
	};

	moveDist.x -= inputVec.x * transSpeed_.x;
	moveDist.y += inputVec.y * transSpeed_.y;
	//�J�������ǂ̂��炢�v���C���[�ɋ߂Â���Clamp������
	moveDist.y = Clamp(moveDist.y, -0.5f, 1.0f);

	//���ʍ��W���
	endEyePos_.x = -frontdist * sinf(moveDist.x) * cosf(moveDist.y) + cameraTrans.x;
	endEyePos_.y = frontdist * sinf(moveDist.y) + cameraTrans.y;
	endEyePos_.z = -frontdist * cosf(moveDist.x) * cosf(moveDist.y) + cameraTrans.z;

	float maxGamecameraY = player_->GetWorldTransform()->position_.y + 25;
	float minGamecameraY = 2.5f;

	camera_->eye_.y = Clamp(camera_->eye_.y, minGamecameraY, maxGamecameraY);

#ifdef _DEBUG
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

	ImGui::End();
#endif // _DEBUG
}

void GameCamera::UpdateLookAT()
{
	if (gameCameraMode_ == GameCameraMode::NORMAL) {
		IEnemy* enemy = player_->GetAttackManager()->GetLockOnEnemy();
		//���b�N�I�����Ă���G������ꍇ�̃J��������
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
					getOutWay = GetOutScreenEnemy(screenPos, windowSize, enemy);
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
	}
	//�Q�[���N���A���̃J�������[�N
	else if (gameCameraMode_ == GameCameraMode::CLEAR) {
		float rate = 0;
		if (clsumSystem_->GetClearType() == ClearType::NextRound) {
			rate = clsumSystem_->GetClearBlankTimer().GetTimeRate();
		}
		else if (clsumSystem_->GetClearType() == ClearType::GameClear) {
			rate = clsumSystem_->GetGameClearBlankTimer().GetTimeRate();
		}


		frontDist_ = Easing::Sine::easeInOut(rate, 15.f, -5.f, 1.0f);
		//�v���C���[�̑O���x�N�g�� * �������v�Z
		Vector3 frontVec = player_->GetPlayerFrontVec() * frontDist_;

		offsetPos_ = {
			Easing::Sine::easeInOut(rate,0.f,randOffsetPos_.x,1.0f),
			Easing::Sine::easeInOut(rate,0.f,randOffsetPos_.y,1.0f),
			0
		};

		Vector3 eyepos = {
			player_->GetWorldTransform()->position_.x + frontVec.x,
			player_->GetWorldTransform()->position_.y + player_->GetWorldTransform()->scale_.y,
			player_->GetWorldTransform()->position_.z + frontVec.z
		};
		eyepos += offsetPos_;

		endEyePos_ = eyepos;
	}

	endTargetPos_ = player_->GetWorldTransform()->position_;
	endTargetPos_.y += player_->GetWorldTransform()->scale_.y;

	camera_->target_ += (endTargetPos_ - camera_->target_) * cameraSpeed_;
}

void GameCamera::Reset()
{
	moveDist = { 0,0.4f,0 };
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
