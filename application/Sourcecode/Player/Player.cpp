#include <imgui.h>
#include "Player.h"
#include "Util.h"
#include "Camera.h"
#include "mSound.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "PlayerDogeRoll.h"
#include "PlayerGuard.h"
#include "PlayerJump.h"
#include "PlayerFreeze.h"
#include "PlayerKnock.h"
#include "PlayerAttack.h"
#include "PlayerAirAttack.h"
#include "PlayerMagic.h"

Player::Player() : IActor()
{

	gravity_.SetAddValue({ 0,-0.01f,0 });

	// ���͂���Ă�������̊p�x
	inputAngle_ = 0.0f;

	//�ړ����x
	moveSpeed_ = 0.2f;
	walklimitValue_ = 0.7f;

	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("player", true));
	//���n�d������
	freezeTimer_.SetLimitTime(7);

	col_.radius = obj_->GetTransform()->scale_.x;
	damageCol_.radius = obj_->GetTransform()->scale_.x;

	damageCoolTime_.SetLimitTime(50);
	mpChargeTime_.SetLimitTime(600);
	mpChargeIntervalTimer_.SetLimitTime(mpChargeTime_.GetLimitTimer() / 20);

	maxHealth_ = 100;
	health_ = maxHealth_;

	maxMP_ = 100;
	nowMP_ = maxMP_;

	isAlive_ = true;
	isMPCharge_ = false;
	guard_.SetPlayer(this);
	knockDecreaseValue = 0.005f;

	sword_.SetAttackManager(command_.GetAttackManager());
	sword_.SetParent(obj_.get());

	command_.SetPlayer(this);

	InitStateMachine();
	GoToState(PlayerState::Idle);
}

void Player::PreUpdate()
{
	addVec_ = { 0,0,0 };
	//�v���C���[�̏�ԍX�V
	StateUpdate();

	////�d��
	GravityUpdate();

	ColPosUpdate();

	Update();

	command_.Update();

	MPCharge();

	damageCoolTime_.AddTime(1);
	hpGaugeUI_.Update(maxHealth_, health_);
	mpGaugeUI_.Update(maxMP_, nowMP_);
}

void Player::PostUpdate()
{
	sword_.Update();

	//�����蔻���gravity�̒l��ω������Ă���
	//PostUpdate��addVec_�ɑ�����Ă���
	ObjUpdate();
	if (command_.GetLockOnEnemy() != nullptr)
	{
		lockOnVec_ =
			command_.GetLockOnEnemy()->GetWorldTransform()->position_ - obj_->GetTransform()->position_;
	}

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
	damageCol_.SetPos(colPos);
}
void Player::InitStateMachine()
{
	//// ��Ԃ�ǉ����Ă����B
	AddState(std::make_shared<PlayerIdle>(this));
	AddState(std::make_shared<PlayerMove>(this));
	AddState(std::make_shared<PlayerDogeRoll>(this));
	AddState(std::make_shared<PlayerGuard>(this));
	AddState(std::make_shared<PlayerJump>(this));
	AddState(std::make_shared<PlayerFreeze>(this));
	AddState(std::make_shared<PlayerKnock>(this));
	AddState(std::make_shared<PlayerAttack>(this));
	AddState(std::make_shared<PlayerAirAttack>(this));
	AddState(std::make_shared<PlayerMagic>(this));
}
#pragma region ����
void Player::InputVecUpdate()
{
	if (GetIsCanMove())
	{

		moveVec_ = { 0,0 };
		Vector3 sideVec;
		Vector3 upVec = { 0,1,0 };
		inputVec_ = { 0,0 };

		//�v���C���[�̐��ʃx�N�g��
		frontVec_ =
			Camera::scurrent_->target_ - Camera::scurrent_->eye_;
		frontVec_.y = 0;
		frontVec_ = frontVec_.normalize();

		sideVec = upVec.cross(frontVec_);
		sideVec = sideVec.normalize();

		// �R���g���[���[���ڑ�����Ă�����
		if (Controller::GetActive())
		{
			float inputlength = 0;
			// ���X�e�B�b�N�̓��͕����x�N�g���擾
			inputVec_ = Controller::GetLStick() / 32768.f;
			inputlength = inputVec_.length();
			//�X�e�B�b�N�̌X������������Ε���
			if (inputlength <= walklimitValue_) {
				inputVec_ = inputVec_.normalize() * 0.5f;
			}
			else {
				inputVec_ = inputVec_.normalize();
			}
		}
		else
		{
			inputVec_ = {
				(float)Key::PushKey(DIK_D) - Key::PushKey(DIK_A),
				(float)Key::PushKey(DIK_W) - Key::PushKey(DIK_S)
			};
		}
		//�J�������猩�����E��O���ړ�
		moveVec_.x = -((frontVec_.z * -inputVec_.x) + (sideVec.z * inputVec_.y));
		moveVec_.y = (frontVec_.z * inputVec_.y) + (sideVec.z * inputVec_.x);
		moveVec_ *= moveSpeed_;

		addVec_ += {moveVec_.x, 0, moveVec_.y};

		/*obj_->GetTransform()->position_ = {
			Clamp(obj_->GetTransform()->position_.x, -77.f, 77.f),
			Clamp(obj_->GetTransform()->position_.y, 0.f, 100.f),
			Clamp(obj_->GetTransform()->position_.z, -77.f, 77.f)
		};*/

		// ���͂��Ă���x�N�g���̊p�x�����߂�
		float inputAngle = Vec2Angle(moveVec_);

		// �v�Z���ʂ��I�[�o�[�t���[���Ă��Ȃ�������l���X�V
		if (inputAngle >= 0)
		{
			inputAngle_ = inputAngle;
		}
		if (Controller::GetLStick().x != 0 ||
			Controller::GetLStick().y != 0)
		{
			obj_->WT_.rotation_ = { 0,Radian(inputAngle_) ,0 };
		}
	}
}

void Player::StateUpdate()
{
	state_ = GetNowState()->GetId();

	freezeTimer_.AddTime(1);
	if (freezeTimer_.GetIsEnd())
	{
		
		if (GetNowState()->GetId() != PlayerState::Knock)
		{
			sword_.SetState(Sword::SwordState::Idle);
			if (command_.GetAttackManager()->GetIsAttacking())
			{
				GoToState(PlayerState::Attack);
				sword_.SetState(Sword::SwordState::Attack);
			}

			if (isFloorCollision_ == false && command_.GetAttackManager()->GetIsAttacking())
			{
				GoToState(PlayerState::AirAttack);
				sword_.SetState(Sword::SwordState::Attack);
			}
		}
	}

	if (guard_.GetIsGurdNow())
	{
		sword_.SetState(Sword::SwordState::Guard);
	}
}

void Player::MPCharge()
{
	//MP����ɂȂ�����
	if (nowMP_ < 0 || nowMP_ == 0) {
		nowMP_ = 0;
		if (isMPCharge_ == false) {
			isMPCharge_ = true;
			mpGaugeUI_.SetIsCharge(isMPCharge_);

		}
	}
	//MP���`���[�W����
	if (isMPCharge_ == true) {
		mpChargeTime_.AddTime(1);
		mpChargeIntervalTimer_.AddTime(1);

		if (mpChargeIntervalTimer_.GetIsEnd()) {
			nowMP_ = (uint32_t)(100.f * mpChargeTime_.GetTimeRate());
			mpChargeIntervalTimer_.Reset();
		}

		if (mpChargeTime_.GetIsEnd()) {
			mpChargeTime_.Reset();
			isMPCharge_ = false;
			mpGaugeUI_.SetIsCharge(isMPCharge_);
			nowMP_ = maxMP_;
		}
	}
}

void Player::DogeRoll()
{
	//�h�b�W���[��
	if (GetIsCanDodge())
	{
		if (Controller::GetTriggerButtons(PAD::INPUT_X))
		{
			dodgeRoll_.Begin(moveVec_.normalize());
			damageCoolTime_.Reset();
			uint32_t limit = dodgeRoll_.GetdodgeTimer().GetLimitTimer();
			damageCoolTime_.SetLimitTime(limit);
			GoToState(PlayerState::DodgeRoll);
		}
	}
}

void Player::DogeRollUpdate()
{
	addVec_ += dodgeRoll_.GetDodgeVec();
	dodgeRoll_.Update();
	if (dodgeRoll_.GetIsDodge() == false) {
		GoToState(PlayerState::Idle);
	}
}

void Player::Guard()
{
	if (GetIsCanGuard())
	{
		if (Controller::GetTriggerButtons(PAD::INPUT_X))
		{
			//�󒆂ɂ���Ƃ��A�m�b�N�o�b�N�̎��U���̎��̓K�[�h���ł��Ȃ�
			SoundManager::Play("GuardSE", false, 0.5f);
			guard_.Init();
			GoToState(PlayerState::Guard);
		}
	}
}

void Player::GuardUpdate()
{
	guard_.Update();

	if (guard_.GetIsGurdNow())
	{
		sword_.SetState(Sword::SwordState::Guard);
	}
	else {
		GoToState(PlayerState::Idle);
	}
}

void Player::Jump()
{
	if (Controller::GetTriggerButtons(PAD::INPUT_A)) {
		//�W�����v������W�����v�\�t���O��false�ɂ���
		isCanJump_ = false;
		GoToState(PlayerState::Jump);
	}
}

void Player::JumpUpdate()
{
	float jumpSpeed = 0.2f;
	int Maxjumptimer = 10;
	//A�������������������W�����v����
	if (Controller::GetButtons(PAD::INPUT_A))
	{
		if (jumpTime_ < Maxjumptimer)
		{
			jumpTime_++;

			gravity_.SetGrabity({ 0, jumpSpeed ,0 });
		}
	}
	//�r����A�𗣂����璅�n����܂ŃW�����v�ł��Ȃ��悤�ɂ���
	if (Controller::GetReleasButtons(PAD::INPUT_A))
	{
		jumpTime_ = Maxjumptimer;
	}
}

void Player::Freeze(uint32_t time)
{
	GoToState(PlayerState::Freeze);
	freezeTimer_.SetLimitTime(time);
	freezeTimer_.Reset();
}

void Player::FreezeUpdate()
{
	freezeTimer_.AddTime(1);

	if (freezeTimer_.GetIsEnd()) {
		GoToState(PlayerState::Idle);
	}
}

void Player::Draw()
{
	Model::lightGroup_->SetCircleShadowCasterPos(0, obj_->WT_.position_);
	obj_->Draw();

	sword_.Draw();


#ifdef _DEBUG
	DrawImGui();

	command_.GetAttackManager()->DrawDebug();
	//guard_.DrawDebug();
	command_.Draw();
#endif // _DEBUG
}

void Player::DrawImGui()
{
	ImGui::Begin("Player");
	/* �����ɒǉ�������GUI������ */
	// Menu Bar
	float pos[3] = {
		obj_->GetTransform()->position_.x,
		obj_->GetTransform()->position_.y,
		obj_->GetTransform()->position_.z,
	};
	ImGui::Text("Pos x: %.2f y: %.2f z: %.2f ", pos[0], pos[1], pos[2]);
	//��]
	float rot[3] = {
		obj_->GetTransform()->rotation_.x,
		obj_->GetTransform()->rotation_.y,
		obj_->GetTransform()->rotation_.z,
	};
	ImGui::Text("Rot x: %.2f y: %.2f z: %.2f ", rot[0], rot[1], rot[2]);
	//�X�P�[��
	float scale[3] = {
		obj_->GetTransform()->scale_.x,
		obj_->GetTransform()->scale_.y,
		obj_->GetTransform()->scale_.z,
	};
	ImGui::Text("Scale x: %.2f y: %.2f z: %.2f ", scale[0], scale[1], scale[2]);

	std::string text = "State : ";
	if (GetNowState()->GetId() == PlayerState::Idle)		text += "Idle";
	if (GetNowState()->GetId() == PlayerState::Move)		text += "Move";
	if (GetNowState()->GetId() == PlayerState::Jump)		text += "Jump";
	if (GetNowState()->GetId() == PlayerState::Attack)		text += "Attack";
	if (GetNowState()->GetId() == PlayerState::AirAttack)	text += "AirAttack";
	if (GetNowState()->GetId() == PlayerState::Freeze)		text += "Freeze";
	if (GetNowState()->GetId() == PlayerState::Knock)		text += "Knock";
	if (GetNowState()->GetId() == PlayerState::Guard)		text += "Guard";
	if (GetNowState()->GetId() == PlayerState::DodgeRoll)	text += "DodgeRoll";
	if (GetNowState()->GetId() == PlayerState::Magic)	text += "Magic";

	ImGui::Text(text.c_str());
	float addvec[3] = { addVec_.x,addVec_.y, addVec_.z };
	ImGui::SliderFloat3("addvec", addvec, 0.f, 1.f, "x = %.3f");

	float gravity = gravity_.GetGravityValue().y;
	ImGui::SliderFloat("gravity", &gravity, -1.f, 0.f, "y = %.3f");

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
	hpGaugeUI_.Draw();
	mpGaugeUI_.Draw();
	command_.DrawSprite();
}

void Player::FloorColision(Vector3 pos)
{
	//�O�t���[���Œn�ʂɐڂ��Ă��Ȃ������Ƃ�
	if (isFloorCollision_ == false)
	{
		Freeze(7);
	}
	isFloorCollision_ = true;
	isCanJump_ = true;
	jumpTime_ = 0;

	obj_->WT_.SetPosition(pos);
}

void Player::WallColision()
{
	ColPosUpdate();
	ObjUpdate();
}

#pragma region ���ꂼ��s���ł������
bool Player::GetIsCanMove()
{
	if (isCanMove_ == false) return false;
	auto state = GetNowState()->GetId();
	if (state_ != PlayerState::Attack &&
		state_ != PlayerState::AirAttack &&
		state_ != PlayerState::Knock &&
		state_ != PlayerState::Guard &&
		state_ != PlayerState::DodgeRoll &&
		state_ != PlayerState::Freeze)
	{
		return true;
	}
	return false;
}

bool Player::GetIsCanDodge()
{
	if (state_ == PlayerState::Move &&
		state_ != PlayerState::DodgeRoll)
	{
		if (addVec_.x != 0 ||
			addVec_.y != 0 ||
			addVec_.z != 0)
		{
			return true;
		}
	}
	return false;
}

bool Player::GetIsCanGuard()
{
	if (state_ != PlayerState::Attack &&
		state_ != PlayerState::AirAttack &&
		state_ != PlayerState::Knock &&
		state_ != PlayerState::Guard &&
		state_ != PlayerState::Jump &&
		state_ != PlayerState::DodgeRoll)
	{
		if (inputVec_.x == 0 &&
			inputVec_.y == 0)
		{
			return true;
		}
	}
	return false;
}

bool Player::GetIsCanJump()
{
	if (state_ != PlayerState::Attack &&
		state_ != PlayerState::AirAttack &&
		state_ != PlayerState::Knock &&
		state_ != PlayerState::Guard &&
		state_ != PlayerState::DodgeRoll &&
		command_.GetIsMagicMenu() == false)
	{
		return true;
	}
	return false;
}

bool Player::GetIsCanAttack()
{
	return isCanMove_;
}
#pragma endregion

void Player::Damage(int32_t damage, Vector3 knockVec)
{
	health_ -= damage;
	knockVec_ = knockVec;
	damageCoolTime_.Reset();
	hpGaugeUI_.Damage();
	GoToState(PlayerState::Knock);
	SoundManager::Play("HitSE", false, 0.5f);
	guard_.SetisGurdNow_(false);
	
}

void Player::GuardHit(Vector3 knockVec)
{
	knockVec_ += knockVec;
	guard_.GuardHit();
	damageCoolTime_.Reset();
	damageCoolTime_.SetLimitTime(50);
}

void Player::Reset()
{
	obj_->WT_.position_ = { 0,0,0 };
	command_.SetLockOnEnemy(nullptr);
	inputAngle_ = 0;
	obj_->WT_.rotation_ = { 0,Radian(inputAngle_) ,0 };
}
