#include <imgui.h>
#include "Player.h"
#include "Util.h"
#include "Camera.h"

Player::Player()
{
	controller_ = Controller::GetInstance();

	colPos_.radius = 1;

	// ���͂���Ă�������̊p�x
	inputAngle_ = 0.0f;

	//�ړ����x
	moveSpeed_ = 0.3f;

	model_ = std::move(std::make_unique<Object3d>());
	model_->SetModel(Model::CreateOBJ_uniptr("player", true));

	scale_ = { 1,1,1 };

	info_.frontVec = &frontVec_;
	info_.WT = model_->GetTransform();
	info_.addVec_ = &addVec_;
}

void Player::Update()
{
	addVec_ = { 0,0,0 };
	// ���͕����x�N�g�����X�V
	InputVecUpdate();
	// ���͕����̊p�x���X�V
	InputAngleUpdate();
	//�W�����v
	JumpUpdate();

	attack_.Update();

	//�d��
	GravityUpdate();

	ColPosUpdate();

	// ��]���
	rot_ = { 0,Radian(inputAngle_) ,0 };

	model_->WT_.rotation_ = rot_;
	model_->WT_.position_ += addVec_;
	model_->WT_.scale_ = scale_;
	model_->Update();
	
}
void Player::ColPosUpdate()
{
	model_->WT_.rotation_ = rot_;
	model_->WT_.position_ += addVec_;
	model_->WT_.scale_ = scale_;
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
	Vector2 moveVec2;
	Vector3 sideVec;
	Vector3 upVec = { 0,1,0 };
	Vector2 inputVec_;// -> ���͂���Ă���x�N�g��

	//�v���C���[�̐��ʃx�N�g��
	frontVec_ = {
		Camera::scurrent_.target_.x - Camera::scurrent_.eye_.x,
		Camera::scurrent_.target_.y - Camera::scurrent_.eye_.y,
		Camera::scurrent_.target_.z - Camera::scurrent_.eye_.z
	};
	frontVec_.normalize();

	sideVec = upVec.cross(frontVec_);
	sideVec.normalize();

	// �R���g���[���[���ڑ�����Ă�����
	if (controller_->GetActive())
	{
		// ���X�e�B�b�N�̓��͕����x�N�g���擾
		inputVec_ = controller_->GetLStick() / 3276.8f;
		//�J�������猩�����E��O���ړ�
		moveVec2.x = (frontVec_.z * -inputVec_.x) + (sideVec.z * inputVec_.y);
		moveVec2.y = (frontVec_.z * inputVec_.y) + (sideVec.z * inputVec_.x);
		moveVec2.x = -moveVec2.x;
		moveVec2 *= 0.03f;
	}

	addVec_ += {moveVec2.x, 0, moveVec2.y};

	model_->GetTransform()->position_ = {
		Clamp(model_->GetTransform()->position_.x, -77.f, 77.f),
		Clamp(model_->GetTransform()->position_.y, 0.f, 100.f),
		Clamp(model_->GetTransform()->position_.z, -77.f, 77.f)
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
	float gravitySpeed = -0.015f;
	//���ɐG��Ă�����
	if (isFloorCollision_)
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
	isFloorCollision_ = false;
}

void Player::JumpUpdate()
{
	float jumpSpeed = 0.2f;
	int Maxjumptimer = 20;
	if (controller_->GetButtons(PAD::INPUT_A))
	{
		if (isJump_ == false)
		{
			if (jumpTime_ < Maxjumptimer)
			{
				jumpTime_++;
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
	addVec_ += { 0, gravity_, 0 };
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

	ImGui::SliderFloat("Max X", &MaxMinX_.x, 0.f, 80.f, "x = %.3f");
	ImGui::SliderFloat("Min X", &MaxMinX_.y, -80.f, 0.f, "y = %.3f");

	ImGui::SliderFloat("Max Y", &MaxMinY_.x, 0.f, 80.f, "x = %.3f");
	ImGui::SliderFloat("Min Y", &MaxMinY_.y, -80.f, 0.f, "y = %.3f");


	//ImGui::SliderFloat("front.x", &resultVec2.x, 0.0f, 2000.0f, "x = %.3f");
	//ImGui::SliderFloat("front.y", &resultVec2.y, 0.0f, 2000.0f, "y = %.3f");

	//ImGui::SliderFloat("input.x", &inputVec_.x, -10.0f, 10.0f, "x = %.3f");
	//ImGui::SliderFloat("input.y", &inputVec_.y, -10.0f, 10.0f, "y = %.3f");

	ImGui::End();

}

void Player::floorColision()
{
	isFloorCollision_ = true;
	isJump_ = false;
	jumpTime_ = 0;
}
