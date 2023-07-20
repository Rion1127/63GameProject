#include <imgui.h>
#include "Player.h"
#include "Util.h"
#include "Camera.h"

Player::Player()
{
	controller_ = Controller::GetInstance();

	col_.radius = 1;
	gravity_.SetAddValue({ 0,-0.01f,0 });

	// ���͂���Ă�������̊p�x
	inputAngle_ = 0.0f;

	//�ړ����x
	moveSpeed_ = 0.3f;

	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("player", true));
	//���n�d������
	landingTimer_.SetLimitTime(7);

	attack_.SetPlayer(&state_);
	damageCoolTime_.SetLimitTime(50);
	maxHealth_ = 100;
	health_ = maxHealth_;
	isAlive_ = true;
}

void Player::PreUpdate()
{
	addVec_ = { 0,0,0 };
	//�v���C���[�̏�ԍX�V
	StateUpdate();
	//�U�����Ă���Ƃ��@&& ���n���Ă���Ƃ�
	if (GetIsCanMove() &&
		state_ != PlayerState::Landing)
	{
		// ���͕����x�N�g�����X�V
		InputVecUpdate();
	}

	//�d��
	GravityUpdate();

	ColPosUpdate();

	damageCoolTime_.AddTime(1);
	hpGauge_.Update(maxHealth_,health_);
}

void Player::PostUpdate()
{
	if (GetIsCanMove())
	{
		JumpUpdate();
	}
	//�����蔻���gravity�̒l��ω������Ă���
	//PostUpdate��addVec_�ɑ�����Ă���
	attack_.Update();

	ObjUpdate();

#ifdef _DEBUG
	if (Key::TriggerKey(DIK_1))
	{
		health_ = 100;
	}
	if (Key::TriggerKey(DIK_2))
	{
		Damage(10, {0,0,0});
	}
#endif // _DEBUG


	if (health_ <= 0)
	{
		isAlive_ = false;
	}
}

void Player::GravityUpdate()
{
	gravity_.SetAddValue({ 0,-0.01f,0 });
	gravity_.Update();
}
void Player::ColPosUpdate()
{
	//���f���̌��_�����ɂ��Ă��邽�߂��̕����炷
	Vector3 colPos = {
		obj_->GetTransform()->position_.x,
		obj_->GetTransform()->position_.y + obj_->WT_.scale_.y,
		obj_->GetTransform()->position_.z,
	};

	col_.SetPos(colPos);
}
#pragma region ����
void Player::InputVecUpdate()
{
	Vector2 moveVec2;
	Vector3 sideVec;
	Vector3 upVec = { 0,1,0 };
	Vector2 inputVec_;// -> ���͂���Ă���x�N�g��

	//�v���C���[�̐��ʃx�N�g��
	frontVec_ =
		Camera::scurrent_->target_ - Camera::scurrent_->eye_;
	frontVec_ = frontVec_.normalize();

	sideVec = upVec.cross(frontVec_);
	sideVec = sideVec.normalize();

	// �R���g���[���[���ڑ�����Ă�����
	if (controller_->GetActive())
	{
		// ���X�e�B�b�N�̓��͕����x�N�g���擾
		inputVec_ = controller_->GetLStick() / 3276.8f;
		//�J�������猩�����E��O���ړ�
		moveVec2.x = (frontVec_.z * -inputVec_.x) + (sideVec.z * inputVec_.y);
		moveVec2.y = (frontVec_.z * inputVec_.y) + (sideVec.z * inputVec_.x);
		moveVec2.x = -moveVec2.x;
		moveVec2 *= 0.01f;
	}

	addVec_ += {moveVec2.x, 0, moveVec2.y};

	obj_->GetTransform()->position_ = {
		Clamp(obj_->GetTransform()->position_.x, -77.f, 77.f),
		Clamp(obj_->GetTransform()->position_.y, 0.f, 100.f),
		Clamp(obj_->GetTransform()->position_.z, -77.f, 77.f)
	};

	// ���͂��Ă���x�N�g���̊p�x�����߂�
	float inputAngle = Vec2Angle(moveVec2);

	// �v�Z���ʂ��I�[�o�[�t���[���Ă��Ȃ�������l���X�V
	if (inputAngle >= 0)
	{
		inputAngle_ = inputAngle;
	}
	if (controller_->GetLStick().x != 0 ||
		controller_->GetLStick().y != 0)
	{
		obj_->WT_.rotation_ = { 0,Radian(inputAngle_) ,0 };
	}
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

				gravity_.SetGrabity({ 0, jumpSpeed ,0 });
			}
		}
	}
	//�����߂��������܂�

	if (controller_->GetReleasButtons(PAD::INPUT_A))
	{
		isJump_ = true;
	}
}

void Player::StateUpdate()
{
	if (state_ != PlayerState::Knock) {
		state_ = PlayerState::Idle;
	}
	landingTimer_.AddTime(1);
	if (landingTimer_.GetIsEnd())
	{
		if (state_ != PlayerState::Knock) {
			if (isFloorCollision_ == false)
			{
				state_ = PlayerState::Jump;
			}
			if (attack_.GetIsAttacking())
			{
				state_ = PlayerState::Attack;
			}
			if (isFloorCollision_ == false && attack_.GetIsAttacking())
			{
				state_ = PlayerState::AirAttack;
			}
		}
	}
	else
	{
		state_ = PlayerState::Landing;
	}
}

void Player::Draw()
{
	Model::lightGroup_->SetCircleShadowCasterPos(0, obj_->WT_.position_);
	obj_->Draw();

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
		float x = obj_->GetTransform()->position_.x;
		float y = obj_->GetTransform()->position_.y;
		float z = obj_->GetTransform()->position_.z;
		ImGui::SliderFloat("pos.x", &x, 0.0f, 2000.0f, "x = %.3f");
		ImGui::SliderFloat("pos.y", &y, 0.0f, 2000.0f, "y = %.3f");
		ImGui::SliderFloat("pos.y", &z, 0.0f, 2000.0f, "y = %.3f");

	}

	//��]
	if (ImGui::CollapsingHeader("Rotation"))
	{
		float rot = obj_->GetTransform()->rotation_.y;
		ImGui::SliderFloat("Rot", &rot, 0.0f, Radian(360), "x = %.3f");
	}

	if (ImGui::CollapsingHeader("Scale"))
	{
		float x = obj_->GetTransform()->scale_.x;
		float y = obj_->GetTransform()->scale_.y;
		float z = obj_->GetTransform()->scale_.z;
		ImGui::SliderFloat("scale.x", &x, 0.0f, 2000.0f, "x = %.3f");
		ImGui::SliderFloat("scale.y", &y, 0.0f, 2000.0f, "y = %.3f");
		ImGui::SliderFloat("scale.y", &z, 0.0f, 2000.0f, "y = %.3f");

	}

	std::string text;
	if (state_ == PlayerState::Idle)text = "Idle";
	if (state_ == PlayerState::Jump)text = "Jump";
	if (state_ == PlayerState::Attack)text = "Attack";
	if (state_ == PlayerState::AirAttack)text = "AirAttack";
	if (state_ == PlayerState::Landing)text = "Landing";
	if (state_ == PlayerState::Knock)text = "Knock";

	ImGui::Text(text.c_str());
	float addvec[3] = { addVec_.x,addVec_.y, addVec_.z };
	ImGui::SliderFloat3("addvec", addvec, 0.f, 80.f, "x = %.3f");

	float gravity = gravity_.GetGravityValue().y;
	ImGui::SliderFloat("gravity", &gravity, -80.f, 0.f, "y = %.3f");

	if (ImGui::Button("Damage"))
	{
		Damage(10, { 0,0,0 });
	}
	if (ImGui::Button("Max_Heal"))
	{
		health_ = 100;
	}
	
	ImGui::End();

}

void Player::DrawSprite()
{
	hpGauge_.Draw();
}

void Player::floorColision()
{
	//�O�t���[���Œn�ʂɐڂ��Ă��Ȃ������Ƃ�
	if (isFloorCollision_ == false)
	{
		state_ = PlayerState::Landing;
		landingTimer_.Reset();
		addVec_ = { 0,0,0 };
	}
	isFloorCollision_ = true;
	isJump_ = false;
	jumpTime_ = 0;
	
	gravity_.SetGrabity({ 0,0,0 });
}

bool Player::GetIsCanMove()
{
	if (state_ != PlayerState::Attack &&
		state_ != PlayerState::AirAttack &&
		state_ != PlayerState::Knock)
	{
		return true;
	}
	return false;
}

void Player::Damage(int32_t damage, Vector3 knockVec)
{
	health_ -= damage;
	knockVec_ = knockVec;
	damageCoolTime_.Reset();
	hpGauge_.Damage();
}
