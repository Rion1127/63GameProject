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
#include "GameSpeed.h"
#include "SoundVolume.h"
#include "ParticleDash.h"
#include "ParticleManager.h"
#include "ParticleLanding.h"

Player::Player() :
	IActor(ActorType::Player)
{

	gravity_.SetAddValue({ 0,-0.01f,0 });

	// 入力されている方向の角度
	inputAngle_ = 0.0f;

	//移動速度
	moveSpeed_ = 0.2f;
	walklimitValue_ = 0.7f;

	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("player", true));
	displayObj_ = std::move(std::make_unique<Object3d>());
	displayObj_->SetModel(Model::CreateOBJ_uniptr("player", true));
	//着地硬直時間
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
	sword_.SetParent(displayObj_.get());

	command_.SetPlayer(this);

	InitStateMachine();
	GoToState(PlayerState::Idle);

	isCanInput_ = true;

	displayObj_->WT_.SetRotType(RotType::Quaternion);

	obj_->WT_.quaternion_ = DirectionToDirection(Vector3(0, 0, 0), Vector3(0, 0, 1));
	shakeTimer_.SetLimitTime(40);
	dashParticleTimer_.SetLimitTime(60);
}

void Player::PreUpdate()
{
	if (GetIsHitStopping())return;
	addVec_ = { 0,0,0 };
	//プレイヤーの状態更新
	StateUpdate();

	////重力
	GravityUpdate();

	ColPosUpdate();

	Update();

	command_.Update();

	MPCharge();

	damageCoolTime_.AddTime(1);
	hpGaugeUI_.Update(maxHealth_, health_);
	mpGaugeUI_.Update(maxMP_, nowMP_);

	playerFrontVec_ = {
				sinf(displayObj_->GetTransform()->rotation_.y),
				0,
				cosf(displayObj_->GetTransform()->rotation_.y),
	};
}

void Player::PostUpdate()
{
	ObjUpdate();

	sword_.Update();

	if (command_.GetLockOnEnemy() != nullptr)
	{
		lockOnVec_ =
			command_.GetLockOnEnemy()->GetWorldTransform()->position_ - obj_->GetTransform()->position_;
	}
	Vector3 displayPos = {
		obj_->GetPos().x,
		obj_->GetPos().y + obj_->GetScale().y,
		obj_->GetPos().z,
	};
	Vector3 displayrot = {
		obj_->GetRot().x,
		obj_->GetRot().y,
		obj_->GetRot().z,
	};
	displayObj_->SetPos(displayPos);
	displayObj_->SetRot(displayrot);
	displayObj_->WT_.SetQuaternion(obj_->WT_.quaternion_);

	displayObj_->Update();

	if (health_ <= 0)
	{
		isAlive_ = false;
	}
}

void Player::GravityUpdate()
{
	Vector3 gravity = {
		0,
		-0.01f * GameSpeed::GetPlayerSpeed(),
		0
	};
	gravity_.SetAddValue(gravity);
	gravity_.Update();
}
void Player::ColPosUpdate()
{
	//モデルの原点を下にしているためその分ずらす
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
	//// 状態を追加していく。
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
#pragma region input
void Player::InputVecUpdate()
{
	if (GetIsCanMove() && isCanInput_)
	{

		moveVec_ = { 0,0 };
		Vector3 sideVec;
		Vector3 upVec = { 0,1,0 };
		inputVec_ = { 0,0 };

		//プレイヤーの正面ベクトル
		frontVec_ =
			Camera::scurrent_->target_ - Camera::scurrent_->eye_;
		frontVec_.y = 0;
		frontVec_ = frontVec_.normalize();

		sideVec = upVec.cross(frontVec_);
		sideVec = sideVec.normalize();

		float inputlength = 0;
		bool isDash = false;
		// コントローラーが接続されていたら
		if (Controller::GetActive())
		{
			// 左スティックの入力方向ベクトル取得
			inputVec_ = Controller::GetLStick() / 32768.f;
			inputlength = inputVec_.length();
			//スティックの傾きが小さければ歩く
			if (inputlength <= walklimitValue_) {
				inputVec_ = inputVec_.normalize() * 0.5f;
				isDash = false;
			}
			else {
				inputVec_ = inputVec_.normalize();
				isDash = true;
			}
		}
		else
		{
			inputVec_ = {
				(float)Key::PushKey(DIK_D) - Key::PushKey(DIK_A),
				(float)Key::PushKey(DIK_W) - Key::PushKey(DIK_S)
			};
		}
		//カメラから見た左右手前奥移動
		moveVec_.x = -((frontVec_.z * -inputVec_.x) + (sideVec.z * inputVec_.y));
		moveVec_.y = (frontVec_.z * inputVec_.y) + (sideVec.z * inputVec_.x);
		moveVec_ *= moveSpeed_;

		addVec_ += {moveVec_.x, 0, moveVec_.y};

		// 入力しているベクトルの角度を求める
		float inputAngle = Vec2Angle(moveVec_);
		Quaternion q1 = { 0,0,0,1.f };
		Quaternion q2 = { 0,0,0,1.f };
		if (Controller::GetLStick().x != 0 ||
			Controller::GetLStick().y != 0)
		{
			inputAngle_ = inputAngle;
			obj_->WT_.rotation_ = { 0,Radian(inputAngle_) ,0 };

			
			float dashRadian = 350 * inputVec_.length() * isDash;
			float radian = Radian(-600 + dashRadian);
			q1 = { 1,0,0,radian };

			float addTime;
			if (isDash)addTime = 1.5f;
			else addTime = 1;

			shakeTimer_.AddTime(addTime);
			

			float shakeRadian =
				UpAndDown(shakeTimer_.GetLimitTimer(), 0.1f, shakeTimer_.GetTimer());
			q2 = { 0,0,shakeRadian,1 };

			if (shakeTimer_.GetIsEnd()) {
				shakeTimer_.Reset();
			}
			if (state_ == PlayerState::Move) {
				dashParticleTimer_.AddTime(addTime);

				if (dashParticleTimer_.GetIsEnd()) {
					
					Vector3 dashParticlePos =
						displayObj_->GetTransform()->position_ - playerFrontVec_;
					dashParticlePos.y = 0;

					std::shared_ptr<OneceEmitter> hitEmitter_ = std::make_shared<OneceEmitter>();
					hitEmitter_->particle = std::make_unique<ParticleDash>();
					hitEmitter_->addNum = 6;
					hitEmitter_->time = 20;
					hitEmitter_->pos = dashParticlePos;
					hitEmitter_->addVec = -playerFrontVec_;
					hitEmitter_->scale = 0.7f;
					ParticleManager::GetInstance()->
						AddParticle("Dash", hitEmitter_);

					dashParticleTimer_.Reset();
				}
			}
			else {
				shakeTimer_.Reset();
			}
		}
		else {
			dashParticleTimer_.Reset();
		}
		Vector3 vecY = { 0, 1, 0 };
		auto axisY = MakeAxisAngle(vecY, Radian(inputAngle_));
		axisY = axisY * q1.Conjugate() * q2.Conjugate();
		obj_->WT_.quaternion_ = obj_->WT_.quaternion_.Slerp(axisY, 0.2f);
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
	//MPが空になったら
	if (nowMP_ < 0 || nowMP_ == 0) {
		nowMP_ = 0;
		if (isMPCharge_ == false) {
			isMPCharge_ = true;
			mpGaugeUI_.SetIsCharge(isMPCharge_);

		}
	}
	//MPをチャージする
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
	//ドッジロール
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
			//空中にいるとき、ノックバックの時攻撃の時はガードができない
			SoundManager::Play("GuardSE", false, SoundVolume::GetValumeSE());
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
		//ジャンプしたらジャンプ可能フラグをfalseにする
		isCanJump_ = false;
		GoToState(PlayerState::Jump);
	}
}

void Player::JumpUpdate()
{
	float jumpSpeed = 0.2f;
	float Maxjumptimer = 10;
	//Aを押し続けた分高くジャンプする
	if (Controller::GetButtons(PAD::INPUT_A))
	{
		if (jumpTime_ < Maxjumptimer)
		{
			jumpTime_ += 1 * GameSpeed::GetPlayerSpeed();

			gravity_.SetGrabity({ 0, jumpSpeed ,0 });
		}
	}
	//途中でAを離したら着地するまでジャンプできないようにする
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
	Model::lightGroup_->SetCircleShadowCasterPos(0, displayObj_->WT_.position_);
	displayObj_->Draw();

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
	/* ここに追加したいGUIを書く */
	// Menu Bar
	float pos[3] = {
		obj_->GetTransform()->position_.x,
		obj_->GetTransform()->position_.y,
		obj_->GetTransform()->position_.z,
	};
	ImGui::Text("Pos x: %.2f y: %.2f z: %.2f ", pos[0], pos[1], pos[2]);
	//回転
	float rot[3] = {
		obj_->GetTransform()->rotation_.x,
		obj_->GetTransform()->rotation_.y,
		obj_->GetTransform()->rotation_.z,
	};
	ImGui::Text("Rot x: %.2f y: %.2f z: %.2f ", rot[0], rot[1], rot[2]);
	//スケール
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

	ImGui::SliderFloat("inputAngle", &inputAngle_, 0.f, 3.1415f, "x = %.3f");
	ImGui::SliderFloat("endRot", &goalinputAngle_, 0.f, 3.1415f, "x = %.3f");

	float value[4] = {
		obj_->WT_.quaternion_.x,
		obj_->WT_.quaternion_.y,
		obj_->WT_.quaternion_.z,
		obj_->WT_.quaternion_.w,
	};
	ImGui::DragFloat4("quaternion", value, 0.1f);

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
	//前フレームで地面に接していなかったとき
	if (isFloorCollision_ == false)
	{
		Freeze(7);

		std::shared_ptr<OneceEmitter> hitEmitter_ = std::make_shared<OneceEmitter>();
		hitEmitter_->particle = std::make_unique<ParticleLanding>();
		hitEmitter_->addNum = 6;
		hitEmitter_->time = 20;
		hitEmitter_->pos = pos;
		hitEmitter_->scale = 0.7f;
		ParticleManager::GetInstance()->
			AddParticle("Landing", hitEmitter_);
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

#pragma region GetIsCanMove
bool Player::GetIsCanMove()
{
	if (isCanMove_ == false) return false;
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
	SoundManager::Play("HitSE", false, SoundVolume::GetValumeSE());
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
