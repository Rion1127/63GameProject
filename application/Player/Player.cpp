#include <imgui.h>
#include "Player.h"
#include "Util.h"
#include "Camera.h"

Player::Player()
{
	controller_ = Controller::GetInstance();

	colPos_.radius = 1;

	// ���͂���Ă���x�N�g��
	inputVec_ = { 0.0f, 0.0f };
	// ���͂���Ă�������̊p�x
	inputAngle_ = 0.0f;
	//���X�e�B�b�N���|����Ă��邩
	isLeftStick_ = false;
	//1�t���[���O�̍��X�e�B�b�N���|����Ă��邩
	isOldLeftStick_ = false;

	//�ړ����x
	moveSpeed_ = 0.3f;

	model_ = std::move(std::make_unique<Object3d>());
	model_->SetModel(Model::CreateOBJ_uniptr("player", true));

	scale_ = { 1,1,1 };
}

void Player::Update()
{

	// ���͕����x�N�g�����X�V
	InputVecUpdate();
	// ���͕����̊p�x���X�V
	InputAngleUpdate();
	//�W�����v
	JumpUpdate();

	//�d��
	GravityUpdate();

	ColPosUpdate();

	// ��]���
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

	//���f���̌��_�����ɂ��Ă��邽�߂��̕����炷
	Vector3 colPos = {
		model_->GetTransform()->position_.x,
		model_->GetTransform()->position_.y + 1.0f,
		model_->GetTransform()->position_.z,
	};

	colPos_.SetPos(colPos);

	
}
#pragma region ����
void Player::InputVecUpdate()
{
	//�v���C���[�̐��ʃx�N�g��
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

	// �R���g���[���[���ڑ�����Ă�����
	if (controller_->GetActive())
	{
		// ���X�e�B�b�N�̓��͕����x�N�g���擾
		inputVec_ = controller_->GetLStick() / 3276.8f;
		//�J�������猩�����E��O���ړ�
		moveVec2.x = (resultVec2.y * -inputVec_.x) + (sideVec.z * inputVec_.y);
		moveVec2.y = (resultVec2.y * inputVec_.y) + (sideVec.z * inputVec_.x);
		moveVec2.x = -moveVec2.x;
		moveVec2 *= 0.03f;

		// 1�t���[���O�̏��Ƃ��ĕۑ�
		isOldLeftStick_ = isLeftStick_;

		// ���X�e�B�b�N���|����Ă�����
		if (inputVec_.length() >= 0.5f) isLeftStick_ = true;
		else isLeftStick_ = false;
	}

	pos_ += {moveVec2.x, 0, moveVec2.y};

	pos_ = {
		Clamp(pos_.x, -77.f, 77.f),
		Clamp(pos_.y, 0.f, 100.f),
		Clamp(pos_.z, -77.f, 77.f)
	};

	// ���͂��Ă���x�N�g���̊p�x�����߂�
	float inputAngle = Vec2Angle(moveVec2);

	// �v�Z���ʂ��I�[�o�[�t���[���Ă��Ȃ�������l���X�V
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
	//���ɐG��Ă�����
	if (isFloorCollision)
	{
		gravity_ = 0;
	}
	//�󒆂ɂ�����d�͂𑫂�
	else
	{
		gravity_ += gravitySpeed;
	}

	//model_->GetTransform()->AddPosition(0, gravity_,0);
	//�ʏ펞��false�ɂ��Ă���
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
				//�d�͂��}�C�i�X�ɂ���
				gravity_ = jumpSpeed;
			}
		}
	}
	//�����߂��������܂�

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
	/* �����ɒǉ�������GUI������ */
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

	//��]
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
